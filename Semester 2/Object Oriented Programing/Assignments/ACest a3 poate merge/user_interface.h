#pragma once
#include "services.h"

typedef struct {
    ProductService* service;
} UserInterface;

UserInterface* create_ui(ProductService* service);
void destroy_ui(UserInterface* ui);
void run_ui(UserInterface* ui);

// Helper functions for UI
void print_menu();
void handle_add_product(UserInterface* ui);
void handle_delete_product(UserInterface* ui);
void handle_update_product(UserInterface* ui);
void handle_search_products(UserInterface* ui);
void handle_expiring_products(UserInterface* ui);
void print_products(Product* products, size_t size);
void add_default_products(UserInterface* ui); 