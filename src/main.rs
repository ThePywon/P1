mod error;
mod utility;
mod ecs;
mod rendering;
mod event;
mod p1;

extern crate macros;

use std::ffi::CString;
//use macros::Component;
use glfw::{ffi::{glfwWindowHint, CONTEXT_VERSION_MAJOR, CONTEXT_VERSION_MINOR, OPENGL_CORE_PROFILE, OPENGL_PROFILE}, Action, Context, Key, Modifiers, Scancode};
//use serde::{Serialize, Deserialize};
//use ecs::{Component, Query};
use rendering::{Program, Shader, ShaderKind};
//use p1::P1;

/*#[derive(Serialize, Deserialize, Debug, Component, Clone, Copy)]
struct A {
  position: (i32, i32),
  scale: (i32, i32)
}

#[derive(Component)]
struct B();
#[derive(Component, Debug)]
struct C();
#[derive(Component)]
struct D();
#[derive(Component)]
struct E();
#[derive(Component)]
struct F();
#[derive(Component)]
struct G();
#[derive(Component)]
struct H();
#[derive(Component, Debug)]
struct I();*/


fn key_callback(window: &mut glfw::Window, key: Key, _: Scancode, action: Action, _: Modifiers) {
  if key == Key::Escape && action == Action::Press {
    window.set_should_close(true);
  }
}

fn main() {
  unsafe {
    glfwWindowHint(CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(OPENGL_PROFILE, OPENGL_CORE_PROFILE);
  }

  let mut glfw = glfw::init(glfw::fail_on_errors).unwrap();

  /*let mut engine = P1::new();
  let my_entity = engine.create_entity();
  let entity_b = engine.create_entity();
  let entity_c = engine.create_entity();
  dbg!(my_entity);
  dbg!(entity_b);
  dbg!(entity_c);
  engine.add_component(my_entity, A { position: (1, 2), scale: (3, 4) }).unwrap();
  engine.add_component(my_entity, C {}).unwrap();
  engine.add_component(my_entity, I {}).unwrap();
  //let arc = engine.get_component::<A>(my_entity).unwrap();
  //dbg!(arc.downcast_ref::<A>());

  let system = |mut query: Query<(&mut A, (&C, &I))>| {
    for (a, (c, i)) in query.iter_mut() {
      dbg!(&a);
      dbg!(&c);
      dbg!(&i);
    }
  };

  engine.register_system(system).unwrap();*/

  let (mut window, _) = glfw.create_window(500, 500, "P1 Engine", glfw::WindowMode::Windowed)
    .expect("Failed to create window");

  window.set_key_polling(true);
  window.make_current();
  window.set_key_callback(&key_callback);

  // Load OpenGL library.
  gl_loader::init_gl();
  // Load all the OpenGL function pointer using the `gl` crate.
  gl::load_with(|symbol| gl_loader::get_proc_address(symbol) as *const _);

  let vertices: Vec<f32> = vec![
    -0.5, -0.5, 0.0,  1.0, 0.0, 0.0,
    0.5, -0.5, 0.0,   0.0, 1.0, 0.0,
    0.0, 0.5, 0.0,    0.0, 0.0, 1.0
  ];

  let mut vbo: gl::types::GLuint = 0;
  unsafe { gl::GenBuffers(1, &mut vbo); }

  unsafe {
    gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
    gl::BufferData(gl::ARRAY_BUFFER,
      (vertices.len() * std::mem::size_of::<f32>()) as gl::types::GLsizeiptr,
      vertices.as_ptr() as *const gl::types::GLvoid, gl::STATIC_DRAW);
    // Unbind buffer
    gl::BindBuffer(gl::ARRAY_BUFFER, 0);
  }

  let mut vao: gl::types::GLuint = 0;
  unsafe { gl::GenVertexArrays(1, &mut vao); }

  unsafe {
    gl::BindVertexArray(vao);
    gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
    gl::EnableVertexAttribArray(0);
    gl::VertexAttribPointer(0, 3, gl::FLOAT, gl::FALSE,
    (6 * std::mem::size_of::<f32>()) as gl::types::GLint, std::ptr::null());
    gl::EnableVertexAttribArray(1);
    gl::VertexAttribPointer(1, 3, gl::FLOAT, gl::FALSE,
    (6 * std::mem::size_of::<f32>()) as gl::types::GLint,
    (3 * std::mem::size_of::<f32>()) as *const gl::types::GLvoid);
    gl::BindBuffer(gl::ARRAY_BUFFER, 0);
    gl::BindVertexArray(0);
  }

  let vertex_shader = Shader::from_source(
    &CString::new(include_str!("shader.vert")).unwrap(),
    ShaderKind::Vertex
  ).unwrap();

  let fragment_shader = Shader::from_source(
    &CString::new(include_str!("shader.frag")).unwrap(),
    ShaderKind::Fragment
  ).unwrap();

  let program = Program::from_shaders(
    &[vertex_shader, fragment_shader]
  ).unwrap();

  while !window.should_close() {
    glfw.poll_events();
    window.make_current();
    let (x, y) = window.get_framebuffer_size();
    unsafe {
      gl::Viewport(0, 0, x, y);
      gl::ClearColor(1f32, 1f32, 1f32, 1f32);
      gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
      program.set_used();
      gl::BindVertexArray(vao);
      gl::DrawArrays(gl::TRIANGLES, 0, 3);
      window.swap_buffers();
    }
  }

  println!("Hello, world!");
  
  drop(program);
  
  // Unload the OpenGL library.
  gl_loader::end_gl();
}
