# Client

## Targeted objectives

- Build Linux, Windows, and Switch
- 3D game using voxels, shaders and lights
- Lightweight and fast
- Audio support

## Graphical Library Choice Comparison

|                                                  | **Raylib**                                                                             | **SDL2**                                                                  |
|--------------------------------------------------|----------------------------------------------------------------------------------------|---------------------------------------------------------------------------|
| **Ease of Use**                                  | ✅ Excellent (Simple API, ideal for beginners)                                          | ❓ Moderate (Requires more setup and control)                              |
| **Ready for Game Development**                   | ✅ Yes (Quick to prototype, comprehensive features for 2D/3D games)                     | ❓ Yes (Great for 2D, but requires additional setup for 3D)                |
| **Adapted for Console Homebrews (e.g., Switch)** | ✅ Yes (Supports Raspberry Pi and homebrew consoles like Switch)                        | ❌ No (No native support for console homebrew)                             |
| **3D Handling**                                  | ✅ Built-in (Native 3D support with simple API)                                         | ❌ Not directly (Requires OpenGL or external libraries for 3D)             |
| **Multiplatform Support**                        | ✅ Excellent (Windows, macOS, Linux, Raspberry Pi, and consoles)                        | ✅ Strong (Windows, macOS, Linux, Android, iOS)                            |
| **Performance**                                  | ✅ Optimized for lightweight, fast 2D/3D games                                          | ✅ Highly performant, especially for 2D games and with OpenGL/Vulkan       |
| **Audio Support**                                | ✅ Simple (Built-in audio features with OpenAL)                                         | ✅ Robust (SDL_mixer, supports advanced audio handling)                    |
| **Community and Ecosystem**                      | ✅ Growing (Active community with lots of examples and tutorials)                       | ✅ Established (Large community and many extensions)                       |
| **Documentation**                                | ✅ Excellent (Well-organized, easy to follow for beginners)                             | ✅ Good (Comprehensive but more technical)                                 |
| **Flexibility**                                  | ❓ Moderate (Great for quick development but may lack flexibility for complex projects) | ✅ High (Flexibility for fine-grained control over hardware and rendering) |
| **Use Case for Complex Projects**                | ❌ Limited (Best for small-to-medium scale projects)                                    | ✅ Ideal (Supports complex and large-scale projects)                       |

*Raylib vs SDL2 benchmark based on the expectations and requirements for this client.*

## Conclusion

After evaluating both libraries against our project goals, Raylib is the best fit. Its simplicity and beginner-friendly 
API make it ideal for quickly prototyping and developing a 3D game with voxels, shaders, and lights. Raylib's built-in 
3D support, console homebrew compatibility (including Switch), and lightweight performance are key advantages for our 
cross-platform requirements. While SDL2 offers greater flexibility, Raylib's ease of use and rapid development cycle 
align better with our current needs.