import 'package:flutter/material.dart';
import '../../network/server_api_service.dart';

class ErrorHandler {
  static String getFriendlyMessage(dynamic error) {
    if (error is ServerException) {
      return error.message;
    }
    
    final errorString = error.toString().toLowerCase();
    
    if (errorString.contains('network') || errorString.contains('connection')) {
      return 'Unable to connect to the server. Please check your internet connection and try again.';
    }
    
    if (errorString.contains('timeout')) {
      return 'The request took too long. Please try again.';
    }
    
    if (errorString.contains('not found') || errorString.contains('404')) {
      return 'The requested item could not be found.';
    }
    
    if (errorString.contains('validation') || errorString.contains('invalid')) {
      return 'Please check your input and try again.';
    }
    
    return 'An unexpected error occurred. Please try again later.';
  }

  static void showError(BuildContext context, dynamic error) {
    final message = getFriendlyMessage(error);
    ScaffoldMessenger.of(context).showSnackBar(
      SnackBar(
        content: Text(message),
        backgroundColor: Colors.red,
        duration: const Duration(seconds: 4),
        action: SnackBarAction(
          label: 'OK',
          textColor: Colors.white,
          onPressed: () {
            ScaffoldMessenger.of(context).hideCurrentSnackBar();
          },
        ),
      ),
    );
  }

  static void showSuccess(BuildContext context, String message) {
    ScaffoldMessenger.of(context).showSnackBar(
      SnackBar(
        content: Text(message),
        backgroundColor: Colors.green,
        duration: const Duration(seconds: 2),
      ),
    );
  }
}
