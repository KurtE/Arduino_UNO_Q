Overview and Warning: 
=====
This Arduino library is an experiment and a WIP, which may never go anywhere.

The idea in this library is to allow me to have access to all of the GPIO pins
on the Arduino Zephyr STM32 based boards, such as Giga, Portenta H7, UNO Q

So I have introduced pin names for all of the possible GPIO pins, and then
added digital functions such as digitalWriteFast, digitalReadFast,
digitalToggleFast for all of these pins.  For the fun of it, I also have
Fast version for the actual Arduino pins as well.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


This library uses concepts and relies on the ArduinoCore-zephyr as well as 
Zephyr.
