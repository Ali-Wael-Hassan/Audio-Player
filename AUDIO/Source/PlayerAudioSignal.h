#pragma once
class PlayerAudioSignal
{
	virtual ~PlayerAudioSignal() = default;
	virtual void playBackFinished() = 0;
};