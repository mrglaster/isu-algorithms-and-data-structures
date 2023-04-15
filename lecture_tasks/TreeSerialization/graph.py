import json
import struct


class Node():

    def __init__(self, name: str, value1: float, value2: float):
        self.name = name
        self.value1 = value1
        self.value2 = value2
        self._nodes: list["Node"] = []

    def add_node(self, node: "Node"):
        """Adds node as a child"""
        self._nodes.append(node)

    def to_dict(self):
        """Converts to dictionary"""
        d = dict()
        d["name"] = self.name
        d["value1"] = self.value1
        d["value2"] = self.value2
        d["nodes"] = [node.to_dict() for node in self._nodes]
        return d

    @classmethod
    def from_dict(cls, d):
        """Reads data from dictionary"""
        root = cls(d["name"], d["value1"], d["value2"])
        for node_d in d["nodes"]:
            root.add_node(cls.from_dict(node_d))
        return root

    def to_binary(self):
        bname = self.name.encode()
        bvals = struct.pack(">ff", self.value1, self.value2)
        data = b""
        for node in self._nodes:
            bnode=node.to_binary()
            data+=bnode
        result = struct.pack("i", len(bname)) + bname + bvals + struct.pack("i", len(data)) + data
        return result

    @classmethod
    def from_binary(cls, data):
        len_name = struct.unpack("i",data[:4])[0]
        name = data[4:4+len_name].decode()
        value1, value2 = struct.unpack(">ff",data[4+len_name:len_name+8+4])

        data_childs = data[len_name+8+4+4:]
        len_childs = struct.unpack("i",data[len_name+8+4:len_name+8+4+4])[0]
        root = cls(name, value1, value2)
        cursor = 0
        current_size = len_name+8+4+4+len_childs
        while cursor < len_childs:
            node, node_len = Node.from_binary(data_childs[cursor:])
            root.add_node(node)
            cursor += node_len
        return root, current_size


a = Node("A", 1.5, 2.5)
b = Node("B", 2.5, 3.5)
c = Node("C", 3.5, 4.5)
d = Node("D", 4.5, 5.5)
e = Node("E", 5.5, 6.5)
a.add_node(b)
a.add_node(c)
b.add_node(d)
d.add_node(e)
binary_form = a.to_binary()
print(binary_form)
a_restored = a.from_binary(binary_form)[0].to_dict()
print(a_restored)
