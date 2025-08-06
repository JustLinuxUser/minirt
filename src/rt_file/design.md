# Needs
A real necesity of such file format the ability to represent key-value pairs. 
Another nicity would be storing the structure recursively

IF I treat the list as a JSON list, and then just parse all the entries, and then and only then put them in the correct data type.
This would mean that it would be hard to name the properties explicitly
but, what if there were 2 modes: the normal one, supporting only the ft positional args, and the second JSONish

## supported types
- Vec2 int / float `244,21`
- Vec3 int / float `244,21,24.22`
    - If a float is detected the whole tuple is stored as a float tuple, if no, as an int tuple
    - An int tuple is accepted anywhere where a float tuple is needed
- Ints
- Floats
- dictionaries
- lists
- strings
- No need for comas at all, but we can ignore them as whitespace for convinience

*This is very complex*
I would need a tokenizer, syntax definitions, a bunch of vec definitions, a proper tree parser, and become very complex
And I can do it in:
    - Tokenizer: 1h
    - Parser: 2h
    - Syntax definitions and proper errors: 2h

what if we go back to the original format, but add comments and name comments (like type comments, but just name comments.
This would not support reordering values, but at least it would be easy to read, and the comments will be forced to be correct
Something like

```
L -40.0,50.0,0.0 0.6 10,0,255


L   position:   -40.0,50.0,0.0
    intencity:  0.6
    color:      10,0,255


```


```
# First mode
A
    # some random ratio
    0.2

    # some other random property
    255,255,255

A: {
    # Second mode
    intencity: 0.2
    color: 255,255,255
    attrs: ["checkered" "knackered"]
}

A # Ambiant light
	0.2 # Intencity
	255,255,255 # Color
{
	light_ratio: 0.2,
	color: 255,255,255 # Native support for tuples of the same type
					   # Or we can define a flag for each number wether it's an int or not
}

C 
    coord: -50.0,0,20
    look_at: 0,0,1 
    fov_x: 70
    attributes:
        - Hello

# Using tabulation to separate the 

L -40.0,50.0,0.0 0.6 10,0,255

sp 0.0,0.0,20.6 12.6 10,0,255

pl 0.0,0.0,-10.0 0.0,1.0,0.0 0,0,225

cy 50.0,0.0,20.6    0.0,0.0,1.0 14.2    21.42   10,0,255
```

