# Casio USB MIDI Driver for Mac OS X


This is the home of a non-official MIDI Driver for older, USB-only Casio keyboards that do not work out of the box on Mac OS X. The driver is entirely based on sample code from Apple's SDK, and should work with many models of Casio keyboards. 

Note that this project is entirely unsupported by Casio and is simply a voluntary effort.



The project originated on Google Code, and you can still visit its old home at:

http://code.google.com/p/casiousbmididriver

## Installation

The current version is compatible with macOS 11.0 (Big Sur) and up. While it has been compiled for both Intel and Apple Silicon (M1 and others), I cannot test if it actually works with Apple Silicon. To install it, simply download the .plugin file and copy it to your /Library/Audio/MIDI and Audio Drivers folder :

https://github.com/francoisferland/casiousbmididriver/releases/tag/11.0

You should then be able to plug your keyboard with a USB cable and power it up, it will show up in the Audio & MIDI Setup utility as "Casio USB Keyboard".

## Older versions

There are older versions of the driver available on the release page, but these are provided "as is", as I cannot maintain these older versions easily.

**IMPORTANT NOTICE** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
