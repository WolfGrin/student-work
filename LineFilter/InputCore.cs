using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LineFilter
{
    public partial class InputCore : Form
    {
        //глобальные переменные
        public bool CustemOk = false;   //установленно ли ядро
        public double[,] Core = new double[3, 3];   //пользовательское ядро

        public InputCore()
        {
            InitializeComponent();
        }
        //если пользователь нажа "Отмена"
        private void button2_Click(object sender, EventArgs e)
        {
            CustemOk = false;
            Close();
        }
        //заполнение ядра пользовательскими данными
        private void button1_Click(object sender, EventArgs e)
        {
            bool Valid = true;

            if (!Double.TryParse(textBox1.Text, out Core[0, 0])) Valid = false;
            if (!Double.TryParse(textBox2.Text, out Core[0, 1])) Valid = false;
            if (!Double.TryParse(textBox3.Text, out Core[0, 2])) Valid = false;
            if (!Double.TryParse(textBox4.Text, out Core[1, 0])) Valid = false;
            if (!Double.TryParse(textBox5.Text, out Core[1, 1])) Valid = false;
            if (!Double.TryParse(textBox6.Text, out Core[1, 2])) Valid = false;
            if (!Double.TryParse(textBox7.Text, out Core[2, 0])) Valid = false;
            if (!Double.TryParse(textBox8.Text, out Core[2, 1])) Valid = false;
            if (!Double.TryParse(textBox9.Text, out Core[2, 2])) Valid = false;

            //проверка корректности заполнения ядра
            if (Valid)
            {
                CustemOk = true;
                Close();
            }
            else
            {
                MessageBox.Show("Ошибка заполнения ядра!", "Внимание!", MessageBoxButtons.OK);
                Close();
            }
        }
    }
}
