extern crate gl;
use std::ffi::{CStr, CString};
use super::utility::create_whitespace_cstring_with_len;

pub struct Shader {
  id: gl::types::GLuint
}

#[derive(Debug, Clone, Copy)]
pub enum ShaderKind {
  Vertex = 35633,
  Fragment = 35632,
  Compute = 37305,
  Geometry = 36313
}

// Don't exactly need it so far but convertion to and back from u32 can be useful so definitely keeping it
impl TryFrom<u32> for ShaderKind {
  type Error = ();

  fn try_from(value: u32) -> Result<Self, Self::Error> {
    match value {
      v if v as isize == ShaderKind::Vertex as isize => Ok(ShaderKind::Vertex),
      v if v as isize == ShaderKind::Fragment as isize => Ok(ShaderKind::Fragment),
      v if v as isize == ShaderKind::Compute as isize => Ok(ShaderKind::Compute),
      v if v as isize == ShaderKind::Geometry as isize => Ok(ShaderKind::Geometry),
      _ => Err(())
    }
  }
}

type ShaderError = (ShaderKind, String);

impl Shader {

  pub fn from_source(
    source: &CStr,
    kind: ShaderKind
  ) -> Result<Shader, ShaderError> {
    let id = create_shader_from_source(source, kind)?;
    Ok(Shader { id })
  }

  pub fn id(&self) -> gl::types::GLuint { self.id }
}

impl Drop for Shader {
  fn drop(&mut self) {
    unsafe {
      gl::DeleteShader(self.id);
    }
  }
}

fn create_shader_from_source(source: &CStr, kind: ShaderKind) -> Result<gl::types::GLuint, ShaderError> {
  let id = unsafe { gl::CreateShader(kind as u32) };

  unsafe {
    gl::ShaderSource(id, 1, &source.as_ptr(), std::ptr::null());
    gl::CompileShader(id);
  }

  let mut success: gl::types::GLint = 1;
  unsafe {
    gl::GetShaderiv(id, gl::COMPILE_STATUS, &mut success);
  }

  if success == 0 {
    let mut len: gl::types::GLint = 0;
    unsafe {
      gl::GetShaderiv(id, gl::INFO_LOG_LENGTH, &mut len);
    }
    let error: CString = create_whitespace_cstring_with_len(len as usize);
    unsafe {
      gl::GetShaderInfoLog(id, len,
        std::ptr::null_mut(), error.as_ptr() as *mut gl::types::GLchar);
    }

    return Err((kind, error.to_string_lossy().into_owned()));
  }

  Ok(id)
}
