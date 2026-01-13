use winit::event::{Event, WindowEvent};
use winit::event_loop::{ControlFlow, EventLoop};
use winit::window::WindowBuilder;
use softbuffer::{Context, Surface};
use std::num::NonZeroU32;
use std::ffi::CString;

use std::sync::Arc;

extern "C" {
    fn render_page(html: *const i8, buffer: *mut u32, width: i32, height: i32);
}

fn main() {
    let event_loop = EventLoop::new().unwrap();
    let window = Arc::new(WindowBuilder::new().with_title("Rusty Browser").build(&event_loop).unwrap());
    
    // softbuffer 0.4 usage: Context -> Surface
    // Remove unsafe as per warning
    let context = Context::new(&window).unwrap();
    let mut surface = Surface::new(&context, &window).unwrap();

    let html_content = std::fs::read_to_string("verify.html").unwrap_or_else(|_| "<html><body><h1>Hello</h1><p>World</p></body></html>".to_string());
    let c_html = CString::new(html_content).unwrap();

    let window_loop = window.clone();

    event_loop.run(move |event, target| {
        target.set_control_flow(ControlFlow::Wait);

        match event {
            Event::WindowEvent {
                event: WindowEvent::CloseRequested,
                window_id,
            } if window_id == window_loop.id() => target.exit(),
            
            Event::WindowEvent {
                event: WindowEvent::RedrawRequested,
                window_id,
            } if window_id == window_loop.id() => {
                let (width, height) = {
                    let size = window_loop.inner_size();
                    (size.width, size.height)
                };
                
                if width == 0 || height == 0 {
                    return;
                }
                
                let nzu_width = NonZeroU32::new(width).unwrap();
                let nzu_height = NonZeroU32::new(height).unwrap();

                surface.resize(nzu_width, nzu_height).unwrap();
                
                let mut buffer = surface.buffer_mut().unwrap();
                
                // Call C++ to render into the buffer
                // buffer is a slice of u32, we pass it as a pointer
                unsafe {
                    render_page(
                        c_html.as_ptr(),
                        buffer.as_mut_ptr(),
                        width as i32,
                        height as i32
                    );
                }
                
                buffer.present().unwrap();
            },
            Event::AboutToWait => {
                window_loop.request_redraw();
            }
             _ => {}
        }
    });
}
