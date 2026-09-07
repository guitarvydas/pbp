In Javascript:
s is a string containing a two-level list.
The top level items are separated by "⫶".
Each inner item contains sub-items separated by "◦".
The top level list always contains a trailing "⫶", resulting in an empty final top level item.
example: s = "aaa◦bbb⫶ccc◦ddd⫶"
I want a JS function `first(s)` that .joins('') every first sub-item of every inner item.
And a function `second(s)`  that .joins('') every second sub-item of every inner item.

