#pragma once
class PlayerAudioSignal
{
	public:
		virtual ~PlayerAudioSignal() = default;
		virtual void playBackFinished() = 0;
};