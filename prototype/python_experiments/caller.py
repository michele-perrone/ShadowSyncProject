from foo import Foo
from function import var_changer

model = Foo()

print(model.var)

var_changer(model, 3)

print(model.var)
