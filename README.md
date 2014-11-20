
**WORK HEAVILY IN PROGRESS**

# Libs
* OpenCV
* FLTK

# TODO
* TODO to issues
* Connection + Link => Link
* Add ptr to connected connected node on Link to apply logic later
* Links are in a list to draw them, and in each node, to apply logic later

## Application

* Port from Vala to C++
* Pluggable links support

## Node types

### Primitive values

* Number
* Image
	* RGBA (only?)
	* ~~RGB~~
	* Scalar


### Operation
* Colorize heights :: ImageScalar -> ImageRGBA)
* Blur :: ImageRGBA -> ImageRGBA
* AddIrgba :: ImageRGBA -> Vec3 -> ImageRGBA
* AddIsca :: ImageScalar -> Vec3 -> ImageRGBA
* GenColor :: void -> ImageRGBA
* GenPerlin :: void -> ImageScalar
