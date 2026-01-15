use winit::event::{Event, WindowEvent, ElementState, MouseButton};
use winit::event_loop::EventLoop;
use winit::window::WindowBuilder;
use softbuffer::{Context, Surface};
use std::num::NonZeroU32;
use std::ffi::CString;
use std::sync::Arc;
use tiny_skia::*;
use fontdue::Font;

extern "C" {
    fn render_frame(html: *const i8, buffer: *mut u32, width: i32, height: i32, scale_factor: f64);
}

const BASE_UI_HEIGHT: f32 = 60.0;

fn main() {
    let event_loop = EventLoop::new().unwrap();
    let window = Arc::new(WindowBuilder::new()
        .with_title("Rusty Browser")
        .with_inner_size(winit::dpi::LogicalSize::new(1024.0, 768.0))
        .build(&event_loop)
        .unwrap());
    
    let context = Context::new(&window).unwrap();
    let mut surface = Surface::new(&context, &window).unwrap();

    let url = "index.html".to_string();
    let mut html_content = std::fs::read_to_string(&url).unwrap_or_else(|_| "<html><body><h1>Error</h1><p>Could not load index.html</p></body></html>".to_string());

    // Load font
    let font_data = std::fs::read("C:\\Windows\\Fonts\\arial.ttf").expect("Could not find Arial font");
    let font = Font::from_bytes(font_data, fontdue::FontSettings::default()).unwrap();

    let mut cursor_pos = (0.0, 0.0);
    let mut current_scale_factor = window.scale_factor();

    let window_handle = window.clone();
    let _ = event_loop.run(move |event, _target| {
        match event {
            Event::WindowEvent { event: WindowEvent::CloseRequested, .. } => {
                std::process::exit(0);
            },
            Event::WindowEvent { event: WindowEvent::Resized(size), .. } => {
                if size.width != 0 && size.height != 0 {
                    surface.resize(
                        NonZeroU32::new(size.width).unwrap(),
                        NonZeroU32::new(size.height).unwrap(),
                    ).unwrap();
                }
            },
            Event::WindowEvent { event: WindowEvent::CursorMoved { position, .. }, .. } => {
                cursor_pos = (position.x as f32, position.y as f32);
            },
            Event::WindowEvent { event: WindowEvent::ScaleFactorChanged { scale_factor, .. }, .. } => {
                current_scale_factor = scale_factor;
            },
            Event::WindowEvent { event: WindowEvent::MouseInput { state, button, .. }, .. } => {
                if state == ElementState::Pressed && button == MouseButton::Left {
                    let scaled_ui_height = (BASE_UI_HEIGHT * current_scale_factor as f32) as u32;
                    // Check if clicked address bar (simple collision)
                    if cursor_pos.1 < scaled_ui_height as f32 {
                        // For now, just refresh as a demo of interaction
                        html_content = std::fs::read_to_string(&url).unwrap_or_else(|_| "<html><body><h1>Error</h1><p>Reload failed</p></body></html>".to_string());
                    }
                }
            },
            Event::WindowEvent { event: WindowEvent::RedrawRequested, window_id } if window_id == window_handle.id() => {
                let size = window_handle.inner_size();
                if size.width == 0 || size.height == 0 { return; }

                let mut buffer = surface.buffer_mut().unwrap();
                let scale_factor = current_scale_factor as f32;
                let scaled_ui_height = (BASE_UI_HEIGHT * scale_factor) as u32;

                // 1. Render C++ Content into the Central Area
                let content_height = (size.height as i32) - (scaled_ui_height as i32);
                if content_height > 0 {
                    let c_html = CString::new(html_content.clone()).unwrap();
                    unsafe {
                        render_frame(
                            c_html.as_ptr(),
                            buffer.as_mut_ptr().offset((scaled_ui_height as isize) * (size.width as isize)),
                            size.width as i32,
                            content_height,
                            current_scale_factor,
                        );
                    }
                }

                // 2. Render Modern GUI using tiny-skia on the top UI buffer area
                let mut pixmap = Pixmap::new(size.width, scaled_ui_height).unwrap();
                
                // Toolbar Background Gradient
                let mut paint = Paint::default();
                paint.anti_alias = true;
                
                let gradient = LinearGradient::new(
                    Point::from_xy(0.0, 0.0),
                    Point::from_xy(0.0, scaled_ui_height as f32),
                    vec![
                        GradientStop::new(0.0, Color::from_rgba8(45, 45, 50, 255)),
                        GradientStop::new(1.0, Color::from_rgba8(30, 30, 35, 255)),
                    ],
                    SpreadMode::Pad,
                    Transform::identity(),
                ).unwrap();
                paint.shader = gradient;
                pixmap.fill_rect(Rect::from_xywh(0.0, 0.0, size.width as f32, scaled_ui_height as f32).unwrap(), &paint, Transform::identity(), None);
                paint.shader = Shader::SolidColor(Color::BLACK); // Reset shader

                // Toolbar bottom border (subtle shadow-like line)
                let mut pb = PathBuilder::new();
                pb.move_to(0.0, scaled_ui_height as f32 - 1.0);
                pb.line_to(size.width as f32, scaled_ui_height as f32 - 1.0);
                let path = pb.finish().unwrap();
                paint.set_color_rgba8(20, 20, 25, 255);
                pixmap.stroke_path(&path, &paint, &Stroke::default(), Transform::identity(), None);

                // Buttons
                let btn_y = 12.0 * scale_factor;
                let btn_size = 36.0 * scale_factor;
                let mut x = 15.0 * scale_factor;
                let btn_spacing = 10.0 * scale_factor;
                let font_size = 20.0 * scale_factor;
                let corner_radius = 8.0 * scale_factor;

                let labels = [("←", 10.0), ("→", 10.0), ("↻", 8.0)];
                for (label, x_off) in labels {
                    let _rect = Rect::from_xywh(x, btn_y, btn_size, btn_size).unwrap();
                    let path = {
                        let mut pb = PathBuilder::new();
                        pb.move_to(x + corner_radius, btn_y);
                        pb.line_to(x + btn_size - corner_radius, btn_y);
                        pb.quad_to(x + btn_size, btn_y, x + btn_size, btn_y + corner_radius);
                        pb.line_to(x + btn_size, btn_y + btn_size - corner_radius);
                        pb.quad_to(x + btn_size, btn_y + btn_size, x + btn_size - corner_radius, btn_y + btn_size);
                        pb.line_to(x + corner_radius, btn_y + btn_size);
                        pb.quad_to(x, btn_y + btn_size, x, btn_y + btn_size - corner_radius);
                        pb.line_to(x, btn_y + corner_radius);
                        pb.quad_to(x, btn_y, x + corner_radius, btn_y);
                        pb.finish().unwrap()
                    };
                    
                    // Hover effect
                    if cursor_pos.0 >= x && cursor_pos.0 <= x + btn_size &&
                       cursor_pos.1 >= btn_y && cursor_pos.1 <= btn_y + btn_size {
                        paint.set_color_rgba8(70, 70, 80, 255);
                    } else {
                        paint.set_color_rgba8(55, 55, 65, 255);
                    }
                    
                    pixmap.fill_path(&path, &paint, FillRule::Winding, Transform::identity(), None);
                    
                    // Subtle Border
                    paint.set_color_rgba8(80, 80, 90, 255);
                    pixmap.stroke_path(&path, &paint, &Stroke::default(), Transform::identity(), None);

                    // Label
                    draw_text(&mut pixmap, &font, label, x + x_off * scale_factor, btn_y + 26.0 * scale_factor, font_size, Color::WHITE);

                    x += btn_size + btn_spacing;
                }

                // Address Bar
                let addr_x = x;
                let addr_w = size.width as f32 - x - 20.0 * scale_factor;
                let _addr_rect = Rect::from_xywh(addr_x, btn_y, addr_w, btn_size).unwrap();
                let addr_path = {
                    let mut pb = PathBuilder::new();
                    pb.move_to(addr_x + corner_radius, btn_y);
                    pb.line_to(addr_x + addr_w - corner_radius, btn_y);
                    pb.quad_to(addr_x + addr_w, btn_y, addr_x + addr_w, btn_y + corner_radius);
                    pb.line_to(addr_x + addr_w, btn_y + btn_size - corner_radius);
                    pb.quad_to(addr_x + addr_w, btn_y + btn_size, addr_x + addr_w - corner_radius, btn_y + btn_size);
                    pb.line_to(addr_x + corner_radius, btn_y + btn_size);
                    pb.quad_to(addr_x, btn_y + btn_size, addr_x, btn_y + btn_size - corner_radius);
                    pb.line_to(addr_x, btn_y + corner_radius);
                    pb.quad_to(addr_x, btn_y, addr_x + corner_radius, btn_y);
                    pb.finish().unwrap()
                };

                paint.set_color_rgba8(20, 20, 25, 255);
                pixmap.fill_path(&addr_path, &paint, FillRule::Winding, Transform::identity(), None);
                
                paint.set_color_rgba8(80, 80, 90, 255);
                pixmap.stroke_path(&addr_path, &paint, &Stroke::default(), Transform::identity(), None);

                draw_text(&mut pixmap, &font, &url, addr_x + 15.0 * scale_factor, btn_y + 26.0 * scale_factor, 16.0 * scale_factor, Color::from_rgba8(180, 180, 190, 255));

                // Copy Pixmap to softbuffer
                let data = pixmap.data();
                for i in 0..(size.width * scaled_ui_height) as usize {
                    let r = data[i * 4 + 0] as u32;
                    let g = data[i * 4 + 1] as u32;
                    let b = data[i * 4 + 2] as u32;
                    let a = data[i * 4 + 3] as u32;
                    // softbuffer expects 0x00RRGGBB
                    buffer[i] = (a << 24) | (r << 16) | (g << 8) | b;
                }

                buffer.present().unwrap();
            },
            Event::AboutToWait => {
                window_handle.request_redraw();
            },
            _ => {}
        }
    });
}

fn draw_text(pixmap: &mut Pixmap, font: &Font, text: &str, mut x: f32, y: f32, size: f32, color: Color) {
    for c in text.chars() {
        let (metrics, bitmap) = font.rasterize(c, size);
        
        for row in 0..metrics.height {
            for col in 0..metrics.width {
                let alpha = bitmap[row * metrics.width + col];
                if alpha > 0 {
                    let mut paint = Paint::default();
                    let a = color.alpha() * (alpha as f32 / 255.0);
                    paint.set_color(Color::from_rgba(color.red(), color.green(), color.blue(), a).unwrap());
                    paint.anti_alias = false; // Disable AA for 1px pixels to avoid panic
                    
                    pixmap.fill_rect(
                        Rect::from_xywh(
                            (x + metrics.xmin as f32 + col as f32).floor(), 
                            (y - metrics.height as f32 + row as f32).floor(), 
                            1.0, 1.0
                        ).unwrap(),
                        &paint,
                        Transform::identity(),
                        None
                    );
                }
            }
        }
        x += metrics.advance_width;
    }
}
