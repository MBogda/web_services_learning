# Wanted to use this file for decorator that checks types of parameters,
# but had decided it's not needed, because any other database will do it by itself.
import inspect


# Based on decorator from stackoverflow, see http://stackoverflow.com/a/19684962 @checkargs decorator.
# Allow optional arguments, keyword arguments, typing of not at all arguments and None type for arguments if necessary.
# A little problem: can't specify which parameters can be None and which not.
def check_args(allow_none=False):
    def decorator(func):
        def wrapper(*args, **kwargs):
            for index, arg in enumerate(inspect.getfullargspec(func)[0]):
                if index >= len(args):
                    break
                if (not allow_none or args[index] is not None) and \
                        not isinstance(args[index], func.__annotations__.get(arg, object)):
                    raise TypeError("{} is not of type {}".format(args[index], func.__annotations__[arg]))
            for arg, type_ in func.__annotations__.items():
                if arg in kwargs and (not allow_none or kwargs[arg] is not None) and not isinstance(kwargs[arg], type_):
                    raise TypeError("{} is not of type {}".format(kwargs[arg], type_))
            return func(*args, **kwargs)

        wrapper.__doc__ = func.__doc__
        wrapper.__annotations__ = func.__annotations__   # need I use it?
        return wrapper
    return decorator
