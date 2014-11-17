
**WORK HEAVILY IN PROGRESS**

# Libs
* OpenCV
* FLTK

# TODO

## Application

* Port from Vala to C++
* Pluggable links support

## Node hierarchy

### Primitive values

* Number
* Image
	* RGBA (only?)
	* ~~RGB~~
	* Scalar

### Image generator
* Color (RGBA)
* Perlin (Scalar)

### Operation
* Colorize heights :: ImageScalar -> ImageRGBA)
* Blur :: ImageRGBA -> ImageRGBA
* AddIrgba :: ImageRGBA -> Vec3 -> ImageRGBA
* AddIsca :: ImageScalar -> Vec3 -> ImageRGBA
