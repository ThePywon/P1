#[derive(Debug)]
pub enum ComponentError {
  Unregistered,
  AlreadyExistsForEntity,
  NotFoundForEntity,
  // SHOULD NEVER GET THAT ERROR
  // This error occurs when accessing out of bounds indexes in a component bitmask
  // meaning I either somehow don't catch an unregistered component
  // or pass in unchecked component ids
  InvalidId,
  NotEnoughAllocatedSpace,
  // SHOULD ALSO NEVER HAPPEN!
  // This error occurs when a component was stored under the wrong id
  // and there was an attempt at downcasting it resulting in failure
  IllAllocated
}

#[derive(Debug)]
pub enum EntityError {
  NotFound,
  // SHOULD NEVER GET THAT ERROR
  // Happens when a component wasn't found but the entity thinks it has it
  MismatchedComponentBitmask
}

#[derive(Debug)]
pub enum P1Error {
  Component(ComponentError),
  Entity(EntityError)
}

impl From<ComponentError> for P1Error {
  fn from(value: ComponentError) -> Self {
    P1Error::Component(value)
  }
}
impl From<EntityError> for P1Error {
  fn from(value: EntityError) -> Self {
    P1Error::Entity(value)
  }
}
