#pragma once

class EO_Base
{
public:

	EO_Base();
	virtual ~EO_Base();

	virtual void Start();

	virtual void PreUpdate();
	virtual void Update();
	virtual void PostUpdate();

	virtual void CleanUp();

	void SetEnabled(bool enabled)
	{
		this->enabled = enabled;
		/*for (component: components)
		{
			gameobjects.Enabled(enabled);
		})*/
	}

	bool IsEnabled() const
	{
		return enabled;
	}

private:
	bool enabled = true;

};