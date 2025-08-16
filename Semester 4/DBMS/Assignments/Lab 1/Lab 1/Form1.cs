using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Configuration;

namespace L1DBMSForm
{
    public partial class Form1 : Form
    {
        // Database connection and data handling objects
        SqlConnection conn;                    // Database connection
        SqlDataAdapter daManufacturer;        // Adapter for master table (Manufacturer/Customer)
        SqlDataAdapter daAircraftModel;       // Adapter for detail table (AircraftModel)
        DataSet dset;                         // In-memory cache of the database tables
        SqlCommandBuilder cmdBuilderMaster;   // Generates SQL commands for master table
        SqlCommandBuilder cmdBuilderDetail;   // Generates SQL commands for detail table
        BindingSource bsManufacturer;         // Binds master table to grid
        BindingSource bsAircraftModel;        // Binds detail table to grid with master relation
        private string scenario = "";         // Tracks current view ("" for Manufacturer, "2" for Customer)

        public Form1()
        {
            InitializeComponent();
            this.Text = ConfigurationManager.AppSettings["FormTitle"];
            FillData();  // Load initial data from database
        }

        // Handles the Save Changes button click
        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                // Ensure all edits in the grids are committed to the dataset
                this.dataGridView1.EndEdit();
                this.dataGridView2.EndEdit();
                
                // Save changes from dataset back to the database
                daManufacturer.Update(dset, ConfigurationManager.AppSettings["MasterTable"]);
                daAircraftModel.Update(dset, ConfigurationManager.AppSettings["DetailTable"]);
                
                MessageBox.Show("Changes saved successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex) 
            { 
                MessageBox.Show(ex.Message, "Error at updating database", MessageBoxButtons.OK, MessageBoxIcon.Error); 
            }
        }

        // Loads or reloads data from the database
        void FillData()
        {
            try
            {
                // Create new database connection
                conn = new SqlConnection(getConnectionString());
            }
            catch (Exception ex) 
            { 
                MessageBox.Show(ex.Message, "Error at creating connection", MessageBoxButtons.OK, MessageBoxIcon.Error); 
                return;
            }

            // Get queries and key information from configuration
            string masterQuery = ConfigurationManager.AppSettings["MasterQuery"];
            string detailQuery = ConfigurationManager.AppSettings["DetailQuery"];
            string foreignKey = ConfigurationManager.AppSettings["ForeignKey"];

            // Create data adapters for both tables
            daManufacturer = new SqlDataAdapter(masterQuery, conn);
            daAircraftModel = new SqlDataAdapter(detailQuery, conn);

            // Create new dataset to hold the tables
            dset = new DataSet();

            try
            {
                // Set up command builders for automatic SQL command generation
                cmdBuilderMaster = new SqlCommandBuilder(daManufacturer);
                cmdBuilderDetail = new SqlCommandBuilder(daAircraftModel);

                // Configure master table adapter commands
                daManufacturer.InsertCommand = cmdBuilderMaster.GetInsertCommand();
                daManufacturer.UpdateCommand = cmdBuilderMaster.GetUpdateCommand();
                daManufacturer.DeleteCommand = cmdBuilderMaster.GetDeleteCommand();

                // Configure detail table adapter commands
                daAircraftModel.InsertCommand = cmdBuilderDetail.GetInsertCommand();
                daAircraftModel.UpdateCommand = cmdBuilderDetail.GetUpdateCommand();
                daAircraftModel.DeleteCommand = cmdBuilderDetail.GetDeleteCommand();

                // Load data from database into dataset
                daManufacturer.Fill(dset, ConfigurationManager.AppSettings["MasterTable"]);
                daAircraftModel.Fill(dset, ConfigurationManager.AppSettings["DetailTable"]);

                // Create master-detail relationship between tables
                if (!dset.Relations.Contains("MasterDetail"))
                {
                    dset.Relations.Add("MasterDetail", 
                        dset.Tables[ConfigurationManager.AppSettings["MasterTable"]].Columns[foreignKey],
                        dset.Tables[ConfigurationManager.AppSettings["DetailTable"]].Columns[foreignKey]);
                }

                // Set up data bindings for the grids
                bsManufacturer = new BindingSource();
                bsManufacturer.DataSource = dset.Tables[ConfigurationManager.AppSettings["MasterTable"]];
                bsAircraftModel = new BindingSource(bsManufacturer, "MasterDetail");

                // Bind data sources to grids
                this.dataGridView1.DataSource = bsManufacturer;
                this.dataGridView2.DataSource = bsAircraftModel;

                UpdateLabels();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error loading data", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        // Returns the connection string for the database
        string getConnectionString()
        {
            return "Data Source=DESKTOP-MR70AB0;Initial Catalog=Aircraft DealershipH;Integrated Security=true;";
        }

        // Updates the labels based on current view (Manufacturer or Customer)
        private void UpdateLabels()
        {
            if (scenario == "")
            {
                label1.Text = "Manufacturers";
                label2.Text = "Aircraft Models";
            }
            else
            {
                label1.Text = "Customers";
                label2.Text = "Aircraft Models";
            }
        }

        // Handles the Switch View button click (toggles between Manufacturer and Customer views)
        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                // Toggle between scenarios
                if (scenario == "")
                {
                    scenario = "2";
                }
                else
                {
                    scenario = "";
                }

                // Clean up existing data
                if (dset != null)
                {
                    if (dset.Relations.Contains("MasterDetail"))
                        dset.Relations.Remove("MasterDetail");
                    dset.Clear();
                }

                // Update configuration based on selected scenario
                if (scenario == "2")
                {
                    // Configure for Customer view
                    ConfigurationManager.AppSettings["MasterTable"] = "Customer";
                    ConfigurationManager.AppSettings["DetailTable"] = "AircraftModel";
                    ConfigurationManager.AppSettings["MasterQuery"] = "SELECT * FROM Customer";
                    ConfigurationManager.AppSettings["DetailQuery"] = "SELECT * FROM AircraftModel";
                    ConfigurationManager.AppSettings["ForeignKey"] = "CustomerID";
                }
                else
                {
                    // Configure for Manufacturer view
                    ConfigurationManager.AppSettings["MasterTable"] = "Manufacturer";
                    ConfigurationManager.AppSettings["DetailTable"] = "AircraftModel";
                    ConfigurationManager.AppSettings["MasterQuery"] = "SELECT * FROM Manufacturer";
                    ConfigurationManager.AppSettings["DetailQuery"] = "SELECT * FROM AircraftModel";
                    ConfigurationManager.AppSettings["ForeignKey"] = "ManufacturerID";
                }

                UpdateLabels();

                // Clean up resources before reloading
                if (daManufacturer != null) daManufacturer.Dispose();
                if (daAircraftModel != null) daAircraftModel.Dispose();
                if (cmdBuilderMaster != null) cmdBuilderMaster.Dispose();
                if (cmdBuilderDetail != null) cmdBuilderDetail.Dispose();
                if (bsManufacturer != null) bsManufacturer.Dispose();
                if (bsAircraftModel != null) bsAircraftModel.Dispose();

                // Reload data with new configuration
                FillData();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error switching scenarios", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}
