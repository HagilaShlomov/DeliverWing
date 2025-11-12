
//#include "DB_connection.h"
//#include <iostream>
//#include <mysql/jdbc.h> // Replace "cppconn/driver.h" with this header
//#include "Node.h"
//#include "User.h"
//using namespace std;
//using namespace sql::mysql;
//
//
//
//int DatabaseManager::connect_to_DB()
//{
//   try {
//       driver = get_mysql_driver_instance();
//       con = unique_ptr<sql::Connection>(driver->connect("tcp://localhost:3306",
//           "root", "147963"));
//       /*con = driver->connect("tcp://localhost:3306",
//           "root", "147963");*/
//
//       con->setSchema("conecttocpp"); 
//
//       sql::Statement* stmt;
//       stmt = con->createStatement();
//
//       // Uncomment and modify the following code as needed
//       // string createTableSQL = "CREATE TABLE IF NOT EXISTS GFGCourses (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, courses VARCHAR(255) NOT NULL)";
//       // stmt->execute(createTableSQL);
//
//       // string insertDataSQL = "INSERT INTO GFGCourses (courses) VALUES ('DSA'),('C++'),('JAVA'),('PYTHON')";
//       // stmt->execute(insertDataSQL);
//
//       // string selectDataSQL = "SELECT * FROM GFGCourses";
//       // sql::ResultSet* res = stmt->executeQuery(selectDataSQL);
//
//       // int count = 0;
//       // while (res->next()) {
//       //     cout << " Course " << ++count << ": " << res->getString("courses") << endl;
//       // }
//
//       // delete res;
//       // delete stmt;
//       // delete con;
//   }
//   catch (sql::SQLException& e) {
//       std::cerr << "SQL Error: " << e.what() << std::endl;
//   }
//
//   return 0;
//}
//
//void DatabaseManager::createTable()
//{
//	try {
//		if (con) {
//			std::unique_ptr<sql::Statement> stmt(con->createStatement());
//			stmt->execute("CREATE TABLE IF NOT EXISTS weather_data ("
//				"latitude DOUBLE, "
//				"longitude DOUBLE, "
//                "altitude DOUBLE, "
//				"weather_json JSON)");
//			std::cout << "Table weather_data created successfully." << std::endl;
//
//            stmt->execute("CREATE TABLE IF NOT EXISTS user_data ("
//                "username VARCHAR(255), "
//                "password VARCHAR(255), "
//                "city VARCHAR(255), "
//                "street VARCHAR(255), "
//                "house_number VARCHAR(255), ");
//            std::cout << "Table user_data created successfully." << std::endl;
//
//		}
//	}
//	catch (sql::SQLException& e) {
//		std::cerr << "SQL Error: " << e.what() << std::endl;
//	}
//}
//
//void DatabaseManager::insertWeatherData(Graph g)
//{
//    try {
//        if (con) {
//            std::unique_ptr<sql::PreparedStatement> pstmt(
//                con->prepareStatement("INSERT INTO weather_data (latitude, longitude,altitude, weather_json) VALUES (?, ?, ?)")
//            );
//            for (const Node& node : g.adjacency_list) {
//                pstmt->setDouble(1, node.c.x);
//                pstmt->setDouble(2, node.c.y);
//                pstmt->setDouble(3, node.c.z);
//                pstmt->setString(4, node.weatherData.dump());
//                pstmt->executeUpdate();
//                std::cout << "Weather data inserted successfully." << std::endl;
//            }
//        }
//    }
//    catch (sql::SQLException& e) {
//        std::cerr << "SQL Error: " << e.what() << std::endl;
//    }
//}
//
//void DatabaseManager::insertUserData(User user)
//{
//	try {
//		if (con) {
//			std::unique_ptr<sql::PreparedStatement> pstmt(
//				con->prepareStatement("INSERT INTO user_data (username, password) VALUES (?, ?)")
//			);
//			pstmt->setString(1, user.getUsername());
//			pstmt->setString(2, user.getPassword());
//			pstmt->executeUpdate();
//			std::cout << "User data inserted successfully." << std::endl;
//		}
//	}
//	catch (sql::SQLException& e) {
//		std::cerr << "SQL Error: " << e.what() << std::endl;
//	}
//
//}
//
//
//
//
//
//



//json weatherData = json::parse(resultSet->getString("weather_json")); //שליפה מהדאטה בייס
//node.weatherData = weatherData;