use super::Shader;
use super::utility::create_whitespace_cstring_with_len;

pub struct Program {
  id: gl::types::GLuint
}

impl Program {
  pub fn from_shaders(shaders: &[Shader]) -> Result<Self, String> {
    let id = unsafe { gl::CreateProgram() };

    for shader in shaders {
      unsafe { gl::AttachShader(id, shader.id()); }
    }

    unsafe { gl::LinkProgram(id); }

    let mut success: gl::types::GLint = 1;
    unsafe {
      gl::GetProgramiv(id, gl::LINK_STATUS, &mut success);
    }

    // Need proper error handling
    if success == 0 {
      let mut len: gl::types::GLint = 0;
      unsafe {
        gl::GetProgramiv(id, gl::INFO_LOG_LENGTH, &mut len);
      }

      let error = create_whitespace_cstring_with_len(len as usize);

      unsafe {
        gl::GetShaderInfoLog(id, len, std::ptr::null_mut(),
          error.as_ptr() as *mut gl::types::GLchar);
      }

      println!("{}", error.to_string_lossy().into_owned());
      return Err(error.to_string_lossy().into_owned());
    }

    unsafe { gl::LinkProgram(id); }

    for shader in shaders {
      unsafe { gl::DetachShader(id, shader.id()); }
    }

    Ok(Program { id })
  }

  #[allow(dead_code)]
  pub fn id(&self) -> gl::types::GLuint { self.id }

  pub fn set_used(&self) {
    unsafe {
      gl::UseProgram(self.id);
    }
  }
}

impl Drop for Program {
  fn drop(&mut self) {
    unsafe {
      gl::DeleteProgram(self.id);
    }
  }
}
