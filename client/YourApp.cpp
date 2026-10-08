#include "pch.h"
#include "framework.h"
#include "YourApp.h"
#include "YourAppDlg.h"
#include <afxsock.h> // Include for Winsock

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ...existing code...

BOOL CYourApp::InitInstance()
{
    // ...existing code...

    // Initialize Winsock
    if (!AfxSocketInit())
    {
        AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
        return FALSE;
    }

    // Create a socket
    CSocket serverSocket;
    if (!serverSocket.Create(12345)) // Port number
    {
        AfxMessageBox(_T("Failed to create server socket!"));
        return FALSE;
    }

    // Listen for incoming connections
    if (!serverSocket.Listen())
    {
        AfxMessageBox(_T("Failed to listen on server socket!"));
        return FALSE;
    }

    // Accept a client connection
    CSocket clientSocket;
    if (!serverSocket.Accept(clientSocket))
    {
        AfxMessageBox(_T("Failed to accept client connection!"));
        return FALSE;
    }

    // Handle client connection (example: receive data)
    char buffer[1024] = { 0 };
    int bytesReceived = clientSocket.Receive(buffer, sizeof(buffer));
    if (bytesReceived == SOCKET_ERROR)
    {
        AfxMessageBox(_T("Failed to receive data from client!"));
        return FALSE;
    }

    AfxMessageBox(CString(buffer));

    // ...existing code...

    return TRUE;
}

// ...existing code...
