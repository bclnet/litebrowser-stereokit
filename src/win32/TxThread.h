#pragma once

class TxThread {
	HANDLE	m_hThread;
	HANDLE	m_hStop;
	DWORD	m_trdID;

	static DWORD WINAPI sThreadProc(LPVOID lpParameter);
public:
	TxThread();
	virtual ~TxThread();
	virtual DWORD ThreadProc() = 0;

	DWORD	getID() { return m_trdID; }
	void	Run();
	void	Stop();
	BOOL	WaitForStop(DWORD ms);
	void	postMessage(UINT Msg, WPARAM wParam, LPARAM lParam);
};
