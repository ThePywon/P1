#include "Axis.h"
#include "InputManager.h"

namespace P1::inputs {
  Axis::Axis(int positive, int negative) {
    this->positives = {positive};
    this->negatives = {negative};
  }
  Axis::Axis(std::vector<int> positives, std::vector<int> negatives) {
    this->positives = positives;
    this->negatives = negatives;
  }
  Axis::Axis(std::initializer_list<int> positives, std::initializer_list<int> negatives) {
    this->positives = {};
    this->negatives = {};

    for(int positive: positives)
      this->positives.push_back(positive);
    for(int negative: negatives)
      this->negatives.push_back(negative);
  }

  int Axis::get_raw() const {
    int result = 0;

    for(int key: positives)
      if(InputManager::isKeyPressed(key))
        result++;
    for(int key: negatives)
      if(InputManager::isKeyPressed(key))
        result--;
    
    if(result > 1) return 1;
    else if(result < -1) return -1;
    else return result;
  }
}

/*#include "Axis.h"

namespace P1 { namespace inputs {
	void get_raw(Axis* axis);
	void update(Axis *axis);
	
	Axis::Axis(graphics::Window* window, std::vector<int> positives, std::vector<int> negatives) {
		for(std::vector<int>::iterator it = positives.begin(); it != positives.end(); ++it)
			this->positives.push_back(*it);
		for(std::vector<int>::iterator it = negatives.begin(); it != negatives.end(); ++it)
			this->negatives.push_back(*it);
		context = window->input_listener;
		window->input_listener->event_manager->on([this](){get_raw(this);});
		window->event_manager->on(WINDOW_UPDATE_EVENT, [this](){update(this);});
	}
	Axis::Axis(graphics::Window* window, int positive, int negative) {
		positives.push_back(positive);
		negatives.push_back(negative);
		context = window->input_listener;
		window->input_listener->event_manager->on([this](){get_raw(this);});
		window->event_manager->on(WINDOW_UPDATE_EVENT, [this](){update(this);});
	}

	void get_raw(Axis* axis) {
		int result = 0;
		for(std::vector<int>::iterator it = axis->positives.begin(); it != axis->positives.end(); ++it)
			if(axis->context->isKeyPressed(*it)) {
				result++;
				break;
			}
		for(std::vector<int>::iterator it = axis->negatives.begin(); it != axis->negatives.end(); ++it)
			if(axis->context->isKeyPressed(*it)) {
				result--;
				break;
			}
		axis->m_raw = result;
	}

	void update(Axis* axis) {
		axis->update_linear();
		axis->update_smooth();
	}

	Axis2D::Axis2D(graphics::Window* window, std::vector<int> left, std::vector<int> up,
				std::vector<int> right, std::vector<int> down) {
		x = std::make_unique<Axis>(window, right, left);
		y = std::make_unique<Axis>(window, up, down);
	}
	Axis2D::Axis2D(graphics::Window* window, int left, int up, int right, int down) {
		x = std::make_unique<Axis>(window, right, left);
		y = std::make_unique<Axis>(window, up, down);
	}

	void Axis::update_linear() {
		if(m_linear == m_raw) return;

		if(m_linear < m_raw)
			m_linear += speed;
		else if(m_linear > m_raw)
			m_linear -= speed;

		if(m_linear > 1)
			m_linear = 1;
		else if(m_linear < -1)
			m_linear = -1;
		else if(fabs(m_raw - m_linear) <= dead)
			m_linear = m_raw;
	}

	void Axis::update_smooth() {
		if(m_smooth == m_raw) return;

		if(m_smooth < m_raw)
			m_smooth += fabs(m_smooth - m_raw) / smoothness;
		else if(m_smooth > m_raw)
			m_smooth -= fabs(m_smooth - m_raw) / smoothness;

		if(m_smooth > 1)
			m_smooth = 1;
		else if(m_smooth < -1)
			m_smooth = -1;
		else if(fabs(m_raw - m_smooth) <= dead)
			m_smooth = m_raw;
	}
}}
*/