#pragma once

namespace PXG
{
	class AbstractCommand
	{
	public:

		AbstractCommand() = default;
		virtual ~AbstractCommand() = default;

		virtual void Execute() = 0;
		virtual void UnExecute() = 0;

	protected:

		bool commandFinished;	

	private:
	};
}


