import 'dart:io';
import 'package:flutter/foundation.dart';

class ServerConfig {
  static const int defaultPort = 3000;
  static const String? manualServerIp = null;

  static String get baseUrl {
    final ip = _getServerIp();
    return 'http://$ip:$defaultPort';
  }

  static String get wsUrl {
    final ip = _getServerIp();
    return 'ws://$ip:$defaultPort';
  }

  static String _getServerIp() {
    if (manualServerIp != null) {
      return manualServerIp!;
    }

    if (kIsWeb) {
      return 'localhost';
    } else if (Platform.isAndroid) {
      return '10.0.2.2';
    } else if (Platform.isIOS) {
      return 'localhost';
    } else {
      return 'localhost';
    }
  }

  static String getPhysicalDeviceInstructions() {
    return '''
To use with a physical device:

1. Find your computer's IP address:
   - Windows: Run 'ipconfig' in PowerShell
   - Mac/Linux: Run 'ifconfig' in terminal
   - Look for IPv4 address (e.g., 192.168.1.100)

2. Update server_config.dart:
   Change: static const String? manualServerIp = null;
   To:     static const String? manualServerIp = '192.168.1.100';

3. Make sure your phone and computer are on the same Wi-Fi network

4. Hot restart the Flutter app (not just hot reload)
''';
  }
}
