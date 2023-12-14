
                        userAccount->displayTransactions();
                        break;
                    default:
                        cout << "\033[31m[!] Invalid option. Please try again.\033[0m" << endl;
                    }
                } while (optionChar != 'A' && optionChar != 'B');
            }
            else