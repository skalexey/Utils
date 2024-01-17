//  Created by Alexey Skorokhodov on 2024/01/17.
//

#pragma once

namespace utils
{
	class timer
	{
	public:
		timer(float period = .0f) : m_period(period) {}
		bool update(float dt) {
			m_elapsed += dt;
			return m_elapsed < m_period;
		}
		float get_elapsed() const {
			return m_elapsed;
		}
		void reset() {
			m_elapsed = .0f;
		}
		bool is_expired() const {
			return m_elapsed >= m_period;
		}

	private:
		float m_elapsed = .0f;
		float m_period = .0f;
	};
}