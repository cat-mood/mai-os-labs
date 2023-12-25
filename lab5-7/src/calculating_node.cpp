#include "calculating_node.h"

using namespace mysys;

std::vector<int> PrefixFunction(const std::string& s) {
	unsigned int n = s.size();
	std::vector<int> p(n);
	for (int i = 1; i < n; ++i) {
		p[i] = p[i - 1];
		while (p[i] > 0 and s[i] != s[p[i]]) {
			p[i] = p[p[i] - 1];
		}
		if (s[i] == s[p[i]]) {
			++p[i];
		}
	}
	return p;
}

std::vector<int> KMPWeak(const std::string& text, const std::string& pattern) {
	std::vector<int> p = PrefixFunction(pattern);
	int m = pattern.size();
	int n = text.size();
	int i = 0;
	std::vector<int> ans;
	if (m > n) {
		return ans;
	}
	while (i < n - m + 1) {
		int j = 0;
		while (j < m and pattern[j] == text[i + j]) {
			++j;
		}
		if (j == m) {
			ans.push_back(i);
		} else {
			if (j > 0 and j > p[j - 1]) {
				i = i + j - p[j - 1] - 1;
			}
		}
		++i;
	}
	return ans;
}

std::vector<int> CalculatingNode::exec(const std::string& text, const std::string& pattern) {
    return KMPWeak(text, pattern);
}

CalculatingNode::CalculatingNode(int id, int base_port) : 
_id{id},
_base_port{base_port},
_s_parent(_context, zmq::socket_type::pair) {
	_s_parent.set(zmq::sockopt::sndtimeo, 3000);
	std::string addr = "tcp://localhost:" + std::to_string(_base_port + _id);
	_s_parent.connect(addr);
}

CalculatingNode::CalculatingNode(CalculatingNode&& other) noexcept {
	_context = std::move(other._context);
	_s_parent = std::move(other._s_parent);
	_s_children = std::move(other._s_children);
	_base_port = std::move(_base_port);
	_id = std::move(other._id);
}

CalculatingNode::~CalculatingNode() noexcept {
	for (auto& [id, child] : _s_children) {
		delete child;
	}
}

void CalculatingNode::req(zmq::socket_t& child, const MyMessage& msg) {
	zmq::message_t message_type(std::to_string(msg.type));
	if (msg.type == MessageType::ping) {
		auto res = child.send(message_type, zmq::send_flags::dontwait);
	}
	auto res = child.send(message_type, zmq::send_flags::sndmore);
	zmq::message_t message_text(msg.text);
	res = child.send(message_text, zmq::send_flags::none);
}

void CalculatingNode::reply(const MyMessage& msg) {
	zmq::message_t message_type(std::to_string((int) msg.type));
	auto res = _s_parent.send(message_type, zmq::send_flags::sndmore);
	zmq::message_t message_text(msg.text);
	res = _s_parent.send(message_text, zmq::send_flags::none);
}

void CalculatingNode::connect_child(int child_id) {
	zmq::socket_t* child = new zmq::socket_t(_context, zmq::socket_type::pair);
	std::string addr = "tcp://*:" + std::to_string(_base_port + child_id);
	child->bind(addr);
	_s_children.insert(std::make_pair(child_id, child));
}

void CalculatingNode::_msg_to_string(const zmq::message_t& msg, std::string& str) {
	str.resize(msg.size() / sizeof(char));
    std::memcpy(str.data(), msg.data(), msg.size());
}

MyMessage CalculatingNode::get_child_msg(zmq::socket_t& child) {
	MyMessage msg;
	zmq::message_t msg_type;
	// _s_parent.set(zmq::sockopt::rcvtimeo, 3000);
	auto res = child.recv(msg_type);
	// _s_parent.set(zmq::sockopt::rcvtimeo, -1);
	// if (errno == EAGAIN) {
	// 	msg.type = MessageType::error;
	// 	return msg;
	// }
	std::string buf;
	_msg_to_string(msg_type, buf);
	msg.type = (MessageType) std::stoi(buf);
	zmq::message_t msg_text;
	res = child.recv(msg_text);
	_msg_to_string(msg_text, buf);
	msg.text = buf;

	return msg;
}

MyMessage CalculatingNode::get_parent_msg() {
	MyMessage msg;
	zmq::message_t msg_type;
	auto res = _s_parent.recv(msg_type);
	std::string buf;
	_msg_to_string(msg_type, buf);
	msg.type = (MessageType) std::stoi(buf);
	if (msg.type == MessageType::ping) return msg;
	zmq::message_t msg_text;
	res = _s_parent.recv(msg_text);
	_msg_to_string(msg_text, buf);
	msg.text = buf;

	return msg;
}

std::vector<int> CalculatingNode::ping_children() {
	std::vector<int> ids;
	MyMessage msg;
	msg.type = MessageType::ping;
	for (auto& [id, child] : _s_children) {
		req(*child, msg);
		msg = get_child_msg(*child);
		if (msg.type == MessageType::error) ids.push_back(id);
	}
	return ids;
}


