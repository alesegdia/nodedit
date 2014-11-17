
**WORK HEAVILY IN PROGRESS**

# Libs
* OpenCV
* FLTK

# TODO

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
