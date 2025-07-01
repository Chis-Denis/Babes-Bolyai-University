using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data.SqlClient;
using System.Data;

namespace ConsoleApp1
{
    internal class Program
    {
        static void Mains(string[] args)
        {
            /*SqlConnection sqlC = new SqlConnection("Data Source = VLAD; Initial Catalog = SocialMediaApp;Integrated Security = true");
            sqlC.Open();
            SqlCommand command = new SqlCommand("select count(*) from Spies", sqlC);
            Console.WriteLine(command.ExecuteScalar().ToString());
            sqlC.Close();
            */
            SqlConnection sqlC = new SqlConnection("Data Source = VLAD; Initial Catalog = SocialMediaApp;Integrated Security = true");
            sqlC.Open();
            SqlCommand command = new SqlCommand("select count(*) from Spies", sqlC);
            Console.WriteLine(command.ExecuteScalar().ToString());
            command.CommandText = "SELECT * FROM Spies";
            SqlDataReader reader = command.ExecuteReader();
            while (reader.Read())
            {
                Console.WriteLine(reader[1].ToString());
            }

            if (reader != null)
            {
                reader.Close();
            }
            //SqlCommand commands2 = new SqlCommand("INSERT INTO Spies VALUES(45,'Gigi Becali','Calin Georgescu',49.7,45.2)", sqlC);
            //commands2.ExecuteNonQuery();
            //sqlC.Close();

            SqlDataAdapter dataAdapter = new SqlDataAdapter("SELECT * FROM Spies", sqlC);
            DataSet dataSet = new DataSet();
            dataAdapter.Fill(dataSet, "Spies");
            foreach (DataRow row in dataSet.Tables["Spies"].Rows)
            {
                Console.WriteLine(row[1].ToString());
            }

            dataAdapter.DeleteCommand = new SqlCommand("DELETE FROM Spies WHERE SpyID = 45", sqlC);
            dataAdapter.DeleteCommand.Parameters.Add("@SpyID", SqlDbType.Int, 0, "SpyID");
            dataSet.Tables["Spies"].Rows[2].Delete();
            dataAdapter.Update(dataSet, "Spies");


            DataRow dataRow = dataSet.Tables["Spies"].NewRow();
            dataRow[0] = 45;
            dataRow[1] = "Gigi Becali";
            dataRow[2] = "Calin Georgescu";
            dataRow[3] = 49.7;
            dataRow[4] = 45.2;
            dataSet.Tables["Spies"].Rows.Add(dataRow);

            dataAdapter.InsertCommand = new SqlCommand("INSERT INTO Spies VALUES(@SpyID,@SpyName,@SpyCountry,@SpyLatitude,@SpyLongitude)", sqlC);


            sqlC.Close();


        }
    }
}