std::string str = "a";
    std::string tmp = "";
    for (size_t i = 0; i < 20; i++) {
        tmp += str;
        set.insert(tmp);
        set.print();
    }

    tmp = "";
    for (size_t i = 0; i < 20; i++) {
        tmp += str;
        set.erase(tmp);
        set.print();
    }
    
    // for (size_t i = 1000000; i < 10000000; ++i) {
    //     set.find(std::to_string(i));
    // }
    // for (size_t i = 1000000; i < 10000000; ++i) {
    //     set.erase(std::to_string(i));
    // }

    // std::cout << "OK" << std::endl;


      // char operator_ = '\0';
    // std::string value;
    // while (std::cin >> operator_ >> value) {
    //     switch (operator_) {
    //         case INSERT: {
    //             std::cout << (set.insert(value) ? "OK" : "FAIL") << std::endl;
    //             break;
    //         }
    //         case ERASE: {
    //             std::cout << (set.erase(value) ? "OK" : "FAIL") << std::endl;
    //             break;
    //         }
    //         case FIND: {
    //             std::cout << (set.find(value) ? "OK" : "FAIL") << std::endl;
    //             break;
    //         }
    //         default:
    //             break;
    //     }
    // }




    void print() const {
        std::cout << "----- table -----" << std::endl;
        for (size_t i = 0; i < sizeBuckets; ++i) {
            std::cout << i << ": ";
           
            if (!buckets[i].flagEmpty) {
                std::cout <<     "KEY    ";
            } else if (buckets[i].flagEmpty && buckets[i].flagErase) {
                std::cout << "DELETED";
            } else if (buckets[i].flagEmpty && !buckets[i].flagErase) {
                std::cout <<   "EMPTY  ";
            }
            
            std::cout << " | " << buckets[i].valueKey << std::endl;
        }
        std::cout << "_keys_count = " << currSizeBuckets << std::endl;
    }