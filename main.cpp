#include "tgbot.cpp"

int main() {
    TgBot::Bot bot("6088590421:AAEIykS59B6S3lB3R6E2vOdJEGHaGKYX3zg");

    sqlite3* db;
    int rc;
    rc = sqlite3_open("mydatabase.sqlite", &db);
    // Обработка ошибки открытия базы данных
    const char* sql = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT, weight INTEGER, progress INTEGER);";
    char* err_msg = nullptr;
    rc = sqlite3_exec(db, sql, nullptr, nullptr, &err_msg);
    if (rc != 0) {
        printf("Error occured while trying to execute db initialization.\n");
        std::cout << rc << std::endl;
        // Обработка ошибки выполнения запроса
        sqlite3_free(err_msg);
    }
    // Откройте базу данных SQLite
    rc = sqlite3_open("mydatabase.sqlite", &db);
    if (rc != SQLITE_OK) {
        // Обработка ошибки открытия базы данных
        return 1;
    }

    std::set<int> weight_enter_step;
    std::set<int> weight_progress_enter_step;

    TgBot::InlineKeyboardMarkup::Ptr admin_keyboard(new TgBot::InlineKeyboardMarkup);
    std::vector<TgBot::InlineKeyboardButton::Ptr> b1;
    std::vector<TgBot::InlineKeyboardButton::Ptr> b2;
    TgBot::InlineKeyboardButton::Ptr create_group(new TgBot::InlineKeyboardButton), create_trenirovky(new TgBot::InlineKeyboardButton);
    create_group->text = "Создать группу";
    create_trenirovky->text = "Создать тренировку";
    create_group->callbackData = "create_group";
    create_trenirovky->callbackData = "create_trenirovky";
    b1.push_back(create_group);
    b2.push_back(create_trenirovky);
    admin_keyboard->inlineKeyboard.push_back(b1);
    admin_keyboard->inlineKeyboard.push_back(b2);
    //-----------------------------------------------

    
    TgBot::InlineKeyboardMarkup::Ptr user_keyboard(new TgBot::InlineKeyboardMarkup);
    std::vector<TgBot::InlineKeyboardButton::Ptr> v1;
    std::vector<TgBot::InlineKeyboardButton::Ptr> v2;
    std::vector<TgBot::InlineKeyboardButton::Ptr> v3;
    TgBot::InlineKeyboardButton::Ptr progress(new TgBot::InlineKeyboardButton), treni(new TgBot::InlineKeyboardButton), progress_new(new TgBot::InlineKeyboardButton);
    progress->text = "Прогресс";
    progress->callbackData = "progress";
    progress_new->text = "Изменить вес";
    progress_new->callbackData = "progress_new";
    treni->text = "Тренировки";
    treni->callbackData = "treni";
    v1.push_back(progress);
    v2.push_back(treni);
    v3.push_back(progress_new);
    user_keyboard->inlineKeyboard.push_back(v1);
    user_keyboard->inlineKeyboard.push_back(v2);
    user_keyboard->inlineKeyboard.push_back(v3);

    
    bot.getEvents().onCommand("start", [&bot, &admin_keyboard, &weight_enter_step, &db](TgBot::Message::Ptr message) {
        if (message->chat->id == 903412687) {
            bot.getApi().sendMessage(message->chat->id, "Вы являйтесь админом",  false , 0 , admin_keyboard);
        } else {
            User user = {message->chat->id, message->chat->firstName, 0, 0};
            addUser(db, user);
            bot.getApi().sendMessage(message->chat->id, "Введите ваш вес!");
            weight_enter_step.insert(message->chat->id);
        }
    });
    bot.getEvents().onAnyMessage([&weight_enter_step, &weight_progress_enter_step, &user_keyboard, &bot, &db](TgBot::Message::Ptr message) {
        int user_id = message->chat->id;
        if (weight_enter_step.find(user_id) != weight_enter_step.end()) {
            if (is_number(message->text)) {
                User user = {message->chat->id, message->chat->firstName, stoi(message->text), 0};
                updateUserWeight(db, user);
                bot.getApi().sendMessage(message->chat->id, "Здравствуйте, " + message->chat->firstName + ", вас приветствует Fitness Bot, который позволяет планировать свою тренировку и фиксировать результаты.", false , 0 , user_keyboard);
                weight_enter_step.erase(user_id);
            } else {
                bot.getApi().sendMessage(message->chat->id, "Вы ввели не число! Пожалуйста, попробуйте снова.");
            }
            return;
        }
        
        if (weight_progress_enter_step.find(user_id) != weight_progress_enter_step.end()) {
            if (is_number(message->text)) {
                User user = getUser(db, message->chat->id);
                user.progress += stoi(message->text);
                updateUserProgress(db, user);
                bot.getApi().sendMessage(message->chat->id, "Ваш изначальный вес: " + std::to_string(user.weight) + " кг, а ваш текущий вес: " + std::to_string(user.weight - user.progress) + " кг. Итого, вы сбросили " + std::to_string(user.progress) + " кг", false , 0 , user_keyboard);
                weight_progress_enter_step.erase(user_id);
            } else {
                bot.getApi().sendMessage(message->chat->id, "Вы ввели не число! Пожалуйста, попробуйте снова.");
            }
            return;
        }

    });
    bot.getEvents().onCallbackQuery([&bot, &weight_progress_enter_step, &db](TgBot::CallbackQuery::Ptr query) {
        if(query->data == "progress"){
            User user = getUser(db, query->message->chat->id);
            bot.getApi().sendMessage(query->message->chat->id, "Ваш изначальный вес: " + std::to_string(user.weight) + " кг, а ваш текущий вес: " + std::to_string(user.weight - user.progress) + " кг. Итого, вы сбросили " + std::to_string(user.progress) + " кг", false , 0);
        }
        if(query->data == "treni"){
            bot.getApi().sendMessage(query->message->chat->id, "Кардио:\n\n1) Берпи\n2) Отжимания\n3) Приседания\n4) Пресс\n5) Буратино\n6) Скакалка\n7) Бег на месте");
        }
        if(query->data == "progress_new"){
            bot.getApi().sendMessage(query->message->chat->id, "Введите ваш прогресс (в килограммах):");
            weight_progress_enter_step.insert(query->message->chat->id);
        }
        if(query->data == "create_group"){
            bot.getApi().sendMessage(query->message->chat->id, "Введите имя новой группы:");
        }
        if(query->data == "create_trenirovky"){
            bot.getApi().sendMessage(query->message->chat->id, "Введите название новой тренировки");
        }
    });

    bot.getApi().deleteWebhook();

    printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
    TgBot::TgLongPoll longPoll(bot);

    // Получите список всех пользователей
    std::vector<User> users = getAllUsers(db);
    try {
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
    // Закройте базу данных SQLite
    sqlite3_close(db);
    return 0;

};
