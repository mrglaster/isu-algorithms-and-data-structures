from enum import Enum


class Alignment(Enum):
    HORIZONTAL = 1
    VERTICAL = 2


class Widget:

    def __init__(self, parent):
        self.parent = parent
        self.childrens = []
        if self.parent is not None:
            self.parent.add_children(self)

    def add_children(self, children: "Widget"):
        self.childrens.append(children)

    def to_binary(self, generation=0, count_brother=0):
        d = {'MainWindow': 0, 'Layout': 1, 'LineEdit': 2, 'ComboBox': 3}
        result = f"{d[self.__class__.__name__]}{generation}/"
        if isinstance(self, MainWindow):
            result += self.title
        elif isinstance(self, Layout):
            result += '1' if self.alignment.name.endswith('HORIZONTAL') else '2'
            generation += 1 + count_brother
        elif isinstance(self, LineEdit):
            result += f"{self.max_length}"
            generation += 1 + count_brother
        elif isinstance(self, ComboBox):
            result += "/".join(map(str, self.items))
            generation += 1 + count_brother
        result += '//'
        for i, child in enumerate(self.childrens):
            result += child.to_binary(generation, i)
        return result

    @classmethod
    def from_binary(cls, data, number=0, history=None, buf_generation=None):
        if buf_generation is None:
            buf_generation = [0]
        if history is None:
            history = []
        buf = data[number]
        class_name = buf
        h_buf = -1
        match class_name:
            case '0':
                title = ''
                while buf != '/' and data[number + 1] != '/':
                    number += 1
                    buf = data[number]
                    title += buf
                select = MainWindow(title)
                history.append(select)
            case '1':
                number += 1
                h_buf = int(data[number])
                number += 2
                alignment = int(data[number])
                select = Layout(history[h_buf], Alignment(alignment))
                position = 1 + buf_generation[h_buf]
                for i in range(h_buf):
                    position += buf_generation[i]
                history.insert(position, select)
            case '2':
                number += 1
                h_buf = int(data[number])
                max_length = ""
                number += 2
                buf = data[number]
                while buf != '/' and data[number + 1] != '/':
                    max_length += buf
                    number += 1
                    buf = data[number]
                select = LineEdit(history[h_buf], int(max_length))
                if len(buf_generation) < h_buf + 1:
                    buf_generation.append(0)
                position = 1 + buf_generation[h_buf]
                for i in range(h_buf):
                    position += buf_generation[i]
                history.insert(position, select)
            case '3':
                number += 1
                h_buf = int(data[number])
                items = []
                number += 2
                buf = data[number]
                while not (buf == '/' and data[number - 1] == '/'):
                    item = ''
                    while buf != '/':
                        item += buf
                        number += 1
                        buf = data[number]
                    items.append(item)
                    number += 1
                    buf = data[number]
                select = ComboBox(history[h_buf], items)
                if len(buf_generation) < h_buf + 1:
                    buf_generation.append(0)
                position = 1 + buf_generation[h_buf]
                for i in range(h_buf):
                    position += buf_generation[i]
                history.insert(position, select)
                number -= 2
        number += 3
        if len(buf_generation) < h_buf + 1:
            buf_generation.append(1)
        elif h_buf >= 0:
            buf_generation[h_buf] += 1
        if number < len(data):
            cls.from_binary(data, number, history, buf_generation)
        return history[0]

    def __str__(self):
        return f"{self.__class__.__name__}{self.childrens}"

    def __repr__(self):
        return str(self)


class MainWindow(Widget):

    def __init__(self, title: str):
        super().__init__(None)
        self.title = title


class Layout(Widget):

    def __init__(self, parent, alignment: Alignment):
        super().__init__(parent)
        self.alignment = alignment


class LineEdit(Widget):

    def __init__(self, parent, max_length: int = 10):
        super().__init__(parent)
        self.max_length = max_length


class ComboBox(Widget):

    def __init__(self, parent, items):
        super().__init__(parent)
        self.items = items


app = MainWindow("Application")
layout1 = Layout(app, Alignment.HORIZONTAL)
layout2 = Layout(app, Alignment.VERTICAL)

edit1 = LineEdit(layout1, 20)
edit2 = LineEdit(layout1, 30)

box1 = ComboBox(layout2, [1, 2, 3, 4])
box2 = ComboBox(layout2, ["a", "b", "c"])

print(app)

bts = app.to_binary()
print(f"Binary data length {len(bts)}")

new_app = MainWindow.from_binary(bts)
print(new_app)
