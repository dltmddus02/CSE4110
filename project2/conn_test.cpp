#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")

const char* host = "localhost";
const char* user = "root";
const char* pw = "ismyjuuu115!";
const char* db = "new_schema_db";

int main(void) {

	MYSQL* connection = NULL;
	MYSQL conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	const char* query;

	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if (connection == NULL)
	{
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}

	else
	{
		printf("Connection Succeed\n");

		if (mysql_select_db(&conn, db))
		{
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;
		}
		// 여기까지는 연동하는 코드. 건들 필요 없음.

		FILE* fp = NULL;
		fp = fopen("20211569_1.txt", "r");
		if (!fp) {
			printf("not exist file!\n");
			return 1;
		}
		char temp[500];
		while (!feof(fp)) {
			query = fgets(temp, sizeof(temp), fp);
			//printf("%s", query);
			mysql_query(connection, query);
			//printf("%d\n\n", mysql_query(connection, query));
		}
		fclose(fp);

		while (1) {
			int num;
			printf("------- SELECT QUERY TYPES -------\n\n");
			printf("\t1. TYPE 1\n");
			printf("\t2. TYPE 2\n");
			printf("\t3. TYPE 3\n");
			printf("\t4. TYPE 4\n");
			printf("\t5. TYPE 5\n");
			printf("\t0. QUIT\n");
			printf("\nSelect query : ");
			scanf("%d", &num);
			//printf("\n");

			if (num == 0) {
				printf("---- QUIT ----\n\n");
				break;
			}
			else if (num == 1) {
				int num1;
				printf("---- TYPE 1 ----\n");
				printf("truck 1721 is destroyed!\n\n");
				while (1) {
					printf("\n---- Subtypes in TYPE 1 ----\n");
					printf("\t1. TYPE 1-1\n");
					printf("\t2. TYPE 1-2\n");
					printf("\t3. TYPE 1-3\n");
					printf("\nSelect query : ");
					scanf("%d", &num1);

					if (num1 == 1) {
						printf("\n---- TYPE 1-1 ----\n");
						printf("** Find all customers who had a package on the truck at the time of the crash. **\n");
						char query1[300] = "select distinct S.customer_ID from shipment S, tracking T where S.shipment_ID = T.shipment_ID and T.location_ID = '1721' and T.current_location = 'truck' and T.tracking_date = '2023-05-21'";
						//char query1[300] = "select S.customer_ID from shipment S, tracking T where S.shipment_ID = T.shipment_ID and T.loaction_ID = '1721' and T.current_location = 'truck' and date_format(tracking_date, '%y-%m-%d') = '2023-05-21'";
						int state1 = 0;
						state1 = mysql_query(connection, query1);
						//printf("state1 : %d\n", state1);
						if (state1 == 0) {
							printf("customer : ");
							// 23년 5월 21일에 truck 1721에 있던 실려있던 짐들의 고객 출력
							sql_result = mysql_store_result(connection);
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
								printf("%s\t", sql_row[0]);
							}
							mysql_free_result(sql_result);
						}
						printf("\n");
						continue;
					}
					else if (num1 == 2) {
						printf("---- TYPE 1-2 ----\n");
						printf("** Find all recipients who had a package on that truck at the time of the crash. **\n");
						char query2[300] = "select distinct S.receiver_name from shipment S, tracking T where S.shipment_ID = T.shipment_ID and T.location_ID = '1721' and T.current_location = 'truck' and T.tracking_date = '2023-05-21'";
						int state2 = 0;
						state2 = mysql_query(connection, query2);
						if (state2 == 0) {
							printf("receiver name : ");
							sql_result = mysql_store_result(connection);
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
								printf("%s\t", sql_row[0]);
							}
							mysql_free_result(sql_result);
						}
						printf("\n");
						continue;
					}
					else if (num1 == 3) {
						printf("---- TYPE 1-3 ----\n");
						printf("** Find the last successful delivery by that truck prior to the crash. **\n");
						// 1721 충돌 전에 가장 마지막으로 배달된 상품의 정보
						char query3[400] = "select distinct P.package_ID, P.package_content from shipment S, package P, tracking T where T.location_ID = '1721' and T.current_location = 'truck' and T.shipment_ID = S.shipment_ID and S.package_ID = P.package_ID and T.tracking_date = (select MAX(tracking_date) from tracking WHERE location_ID = '1721' and current_location = 'truck' and tracking_date < '2023-05-21')";
						int state3 = 0;
						state3 = mysql_query(connection, query3);
						if (state3 == 0) {
							printf("last delivery : ");
							sql_result = mysql_store_result(connection);
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
								printf("%s-%s\t", sql_row[0], sql_row[1]);
								//printf("%s : %s\t", sql_row[0], sql_row[1]);
							}
							mysql_free_result(sql_result);
						}
						printf("\n");
						continue;
					}
					break;
				}
			}
			else if (num == 2) {
				//int num2;
				printf("---- TYPE 2 ----\n");
				printf("** Find the customer who has shipped the most packages in the certain year. **\n");
				printf("Which Year : ");
				int inputYear;
				scanf("%d", &inputYear);
				char query4[400];
				sprintf(query4, "SELECT C.name AS customer_name FROM customer C JOIN shipment S ON C.customer_ID = S.customer_ID WHERE S.receiving_date >= '%d-01-01' AND S.receiving_date <= '%d-12-31' GROUP BY C.name ORDER BY COUNT(S.package_ID) DESC LIMIT 1;", inputYear, inputYear);
				int state4 = 0;
				state4 = mysql_query(connection, query4);
				if (state4 == 0) {
					printf("most packages : ");
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
						printf("%s\t", sql_row[0]);
					}
					mysql_free_result(sql_result);
				}
				printf("\n\n");
				continue;
			}
			else if (num == 3) {
				printf("---- TYPE 3 ----\n");
				printf("** Find the customer who has shipped the most packages in the past year. **\n");
				char query5[400] = "SELECT S.customer_ID, SUM(P.delivery_charge) AS total_delivery_charge FROM shipment S JOIN payment P ON S.customer_ID = P.customer_ID GROUP BY S.customer_ID ORDER BY total_delivery_charge DESC LIMIT 1;";
				//char query5[400] = "SELECT C.name, SUM(P.delivery_charge) AS total_delivery_charge FROM shipment S JOIN payment P ON S.customer_ID = P.customer_ID, customer C WHERE C.customer_ID = S.customer_ID GROUP BY S.customer_ID ORDER BY total_delivery_charge DESC LIMIT 1;";
				//char query5[400] = "SELECT C.name AS customer_name, SUM(P.delivery_charge) AS total_delivery_charge FROM customer C JOIN shipment S ON C.customer_ID = S.customer_ID JOIN payment P ON C.customer_ID = P.customer_ID GROUP BY C.customer_ID ORDER BY total_delivery_charge DESC LIMIT 1;";
				char customer_ID[10];
				int state5 = 0;
				state5 = mysql_query(connection, query5);
				//printf("state5 : %d\n", state5);
				// 서비스에 따른 배송비 + 추가 배송비 중 젤 많이 내는 customer
				if (state5 == 0) {
					//printf("most delivery : ");
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
						//printf("%s %s\t", sql_row[0], sql_row[1]);
						strcpy(customer_ID, sql_row[0]);
					}
					mysql_free_result(sql_result);
				}

				char query5_1[400] = "SELECT name FROM customer WHERE customer_ID = '";
				strcat(query5_1, customer_ID);
				strcat(query5_1, "'");
				int state5_1 = 0;
				state5_1 = mysql_query(connection, query5_1);
				//printf("state5_1 : %d\n", state5_1);
				// 서비스에 따른 배송비 + 추가 배송비 중 젤 많이 내는 customer
				if (state5_1 == 0) {
					printf("most delivery : ");
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
						printf("%s\t", sql_row[0]);
					}
					mysql_free_result(sql_result);
				}

				printf("\n\n");
				continue;
			}
			else if (num == 4) {
				printf("---- TYPE 4 ----\n");
				printf("** Find those packages that were not delivered within the promised time. **\n");
				char query6[400] = "select S.package_ID, P.package_content from shipment S join package P on S.package_ID = P.package_ID where S.expected_arrival_date < S.arrival_date";
				char package_ID[10];
				int state6 = 0;
				state6 = mysql_query(connection, query6);
				if (state6 == 0) {
					printf("delivered late : \n");
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
						printf("%s %s\n", sql_row[0], sql_row[1]);
						//strcpy(package_ID);
					}
					mysql_free_result(sql_result);
				}
				printf("\n");
				continue;
			}
			else if (num == 5) {
				printf("---- TYPE 5 ----\n");
				printf("** Generate the bill for each customer for the past month. Consider creating several types of bills. **\n");

				while (1) {
					int num7;
					printf("\n---- Subtypes in TYPE 5 ----\n");
					printf("\t1. TYPE 5-1 Customer Bill\n");
					printf("\t2. TYPE 5-2 Service Type Bill\n");
					printf("\t3. TYPE 5-3 Package Type Bill\n");
					printf("Select query : ");
					scanf("%d", &num7);

					if (num7 == 1) {
						char customer_name[30];
						char yearmonth[20];
						char customer_ID[10];
						printf("Enter Customer name : ");
						scanf(" %29[^\n]", customer_name);
						int year, month;
						char date1[11], date2[11];

						printf("Enter Year/Month (Ex : 2023-05) : ");
						scanf("%d-%d", &year, &month);

						sprintf(date1, "%d-%02d-01", year, month);

						if (month == 12) {
							sprintf(date2, "%d-%02d-01", year + 1, 1);
						}
						else {
							sprintf(date2, "%d-%02d-01", year, month + 1);
						}

						//printf("첫 번째 날짜: %s\n", date1);
						//printf("두 번째 날짜: %s\n", date2);

						//printf("%s\n", customer_name);
						char query7[400] = "select customer_ID, address from customer where name = '";
						strcat(query7, customer_name);
						strcat(query7, "'");
						int state7 = 0;
						state7 = mysql_query(connection, query7);
						if (state7 == 0) {
							printf("\n\nGenerating [%s - %s %s] Bill ...\n", date1, date2, customer_name);
							sql_result = mysql_store_result(connection);
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
								printf("%s ID : %s\n", customer_name, sql_row[0]);
								printf("%s address : %s\n", customer_name, sql_row[1]);
								strcpy(customer_ID, sql_row[0]);
							}
							mysql_free_result(sql_result);
						}
						char query7_1[400] = "select sum(price_total) from payment where customer_ID = '";
						strcat(query7_1, customer_ID);
						strcat(query7_1, "'");
						strcat(query7_1, " and payment_date > '");
						strcat(query7_1, date1);
						strcat(query7_1, "'");
						strcat(query7_1, " and payment_date < '");
						strcat(query7_1, date2);
						strcat(query7_1, "'");
						strcat(query7_1, " group by customer_ID");
						//printf("%s\n", query7_1);
						int state7_1 = 0;
						state7_1 = mysql_query(connection, query7_1);
						if (state7_1 == 0) {
							sql_result = mysql_store_result(connection);
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
								printf("%s won to pay.\n", sql_row[0]);
							}
						}
						printf("\n");
						continue;
					}
					else if (num7 == 2) {
						char query7_2[400] = "SELECT s.service_ID, SUM(p.package_value * p.count) AS total_fee FROM shipment s JOIN package p ON s.package_ID = p.package_ID GROUP BY s.service_ID ORDER BY s.service_ID ASC";
						int state7_2 = 0;
						state7_2 = mysql_query(connection, query7_2);
						if (state7_2 == 0) {
							printf("\n\nGenerating [Service Type] Bill ...\n");
							sql_result = mysql_store_result(connection);
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
								printf("%s : %s\n", sql_row[0], sql_row[1]);
							}
							mysql_free_result(sql_result);
						}
						printf("\n");
						continue;
					}
					else if (num7 == 3) {
						char query7_3[400] = "select P.package_content, P.package_value * P.count from package P order by P.package_ID ASC";
						int state7_3 = 0;
						state7_3 = mysql_query(connection, query7_3);
						printf("%d\n", state7_3);
						if (state7_3 == 0) {
							printf("\n\nGenerating [Package Type] Bill ...\n");
							sql_result = mysql_store_result(connection);
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
								printf("%s : %s\n", sql_row[0], sql_row[1]);
							}
							mysql_free_result(sql_result);
						}
						printf("\n");
						continue;
					}
					else break;
				}
				printf("\n");
				continue;
			}
			else break;
		}
	}
	FILE* fp2 = NULL;
	fp2 = fopen("20211569_2.txt", "r");
	if (!fp2) {
		printf("not exist file!\n");
		return 1;
	}
	char temp2[500];
	while (!feof(fp2)) {
		query = fgets(temp2, sizeof(temp2), fp2);
		//printf("%s", query);
		mysql_query(connection, query);
		//printf("%d\n\n", mysql_query(connection, query));
	}
	fclose(fp2);

	return 0;
}