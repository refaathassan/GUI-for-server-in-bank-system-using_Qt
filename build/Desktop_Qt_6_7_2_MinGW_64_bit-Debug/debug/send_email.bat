@echo off
setlocal

:: Set variables
set "TO=%~1"
set "SUBJECT=%~2"
set "BODY=%~3"
set "DELAY_SECONDS=10"  REM Delay between retries (in seconds)

:SEND_EMAIL
:: Check if the 'TO' email address is valid
if "%TO%"=="" (
    echo No recipient email address provided.
    exit /b 1
)

:: Construct the PowerShell command with correct SMTP server details
powershell -Command "Send-MailMessage -From 'refaatmahmoudv@gmail.com' -To '%TO%' -Subject '%SUBJECT%' -Body '%BODY%' -SmtpServer 'smtp.gmail.com' -Port 587 -UseSsl -Credential (New-Object PSCredential('refaatmahmoudv@gmail.com', (ConvertTo-SecureString 'cxzm fqex jalt nbfp' -AsPlainText -Force)))"

:: Check for errors in the PowerShell execution
if %ERRORLEVEL% neq 0 (
    echo Error sending email. Check the PowerShell command for details.

    REM Retry logic
    echo Retrying in %DELAY_SECONDS% seconds...
    timeout /t %DELAY_SECONDS% /nobreak > nul
    goto SEND_EMAIL
)

endlocal

