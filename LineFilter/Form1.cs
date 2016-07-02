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
    public partial class Form1 : Form
    {
        //стекло
        double[,] Core = { { 1, -1, 1 }, { -1, 1, -1 }, { 1, -1, 1 } };
        //размытие
        double[,] Core1 = { { 0.1, 0.1, 0.1 }, { 0.1, 0.1, 0.1 }, { 0.1, 0.1, 0.1 } };
        //увеличение чёткости
        double[,] Core2 = { { -0.1, -0.1, -0.1 }, { -0.1, 2, -0.1 }, { -0.1, -0.1, -0.1 } };
        //увеличение гаммы
        double[,] Core3 = { { 0, 0, 0 }, { 0, 2, 0 }, { 0, 0, 0 } };
        //затемнение
        double[,] Core4 = { { -0.1, 0.1, -0.1 }, { 0.1, 0.5, 0.1 }, { -0.1, 0.1, -0.1 } };
        //Фильтр повышения резкости
        double[,] Core5 = { { -1, -1, -1 }, { -1, 16, -1 }, { -1, -1, -1 } };
        //Превитта
        double[,] Core6 = { { -1, 0, 1 }, { -1, 0, 1 }, { -1, 0, 1 } };
        //Собеля
        double[,] Core7 = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
        //Лапласа
        double[,] Core8 = { { -1, -1, -1 }, { -1, 8, -1 }, { -1, -1, -1 } };
        //Гаусса
        double[,] Core9 = { { 1, 2, 1 }, { 2, 4, 2 }, { 1, 2, 1 } };

        public Form1()
        {
            InitializeComponent();
        }
        //применение фильстра
        public void SetFilter(double[,] Core)
        {
            Color RGB;
            Bitmap BmapTmp = new Bitmap(Bmap);
            double r, g, b;
            //проход по пикселям картинки
            for (int x = 1; x < Bmap.Width - 1; x++)
            {
                for (int y = 1; y < Bmap.Height - 1; y++)
                {
                    r = g = b = 0;
                    //вычисление нового значения якоря
                    for (int i = x - 1, k = 0; i <= x + 1; i++, k++)
                    {
                        for (int j = y - 1, h = 0; j <= y + 1; j++, h++)
                        {
                            RGB = BmapTmp.GetPixel(i, j);
                            r += RGB.R * Core[k, h];
                            g += RGB.G * Core[k, h];
                            b += RGB.B * Core[k, h];
                        }
                    }

                    RGB = Color.FromArgb(
                        DIAP((int)(r)), 
                        DIAP((int)(g)),
                        DIAP((int)(b))
                        );
                    //установка нового значения пикселя
                    Bmap.SetPixel(x, y, RGB);

                }
            }
            //замена исходной картинки на картинку с примененым фильтром
            pictureBox1.Image = Bmap;
        }
        //меню -> выход
        private void выходToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult rsl = MessageBox.Show("Вы действительно хотите выйти из приложения?", "Внимание!", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            // если пользователь нажал кнопку да 
            if (rsl == DialogResult.Yes)
            { // выходим из приложения 
                Application.Exit();
            }
        }
        //меню -> открыть изображение
        private void открытьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //загрузка файла
            LoadImage();
        }
        //проверка крайних значений цвета (0 - 255)
        int DIAP(int value)
        {
            return value < 0 ? 0 : (value > 255 ? 255 : value);
        }

        Bitmap Bmap;
        // функция загрузки изображения 
        private void LoadImage()
        {
            // директория, которая будет выбрана как начальная в окне для выбора файла 
            openFileDialog1.InitialDirectory = "%USERPROFILE%\\Pictures";

            // устанавливаем формат файлов для загрузки - jpg 
            openFileDialog1.Filter = "image (JPEG) files (*.jpg)|*.jpg|All files (*.*)|*.*";
           
            // если открытие окна выбора файла завершилось выбором файла и нажатием кнопки ОК 
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                try // безопасная попытка 
                {
                    // пытаемся загрузить файл с именем openFileDialog1.FileName - выбранный пользователем файл. 
                    Bmap = new Bitmap(openFileDialog1.FileName);

                    // устанавливаем картинку в поле элемента PictureBox 
                    pictureBox1.Image = Bmap;
                }
                catch (Exception ex) // если попытка загрузки не удалась 
                {
                    // выводим сообщение с причиной ошибки 
                    MessageBox.Show("Не удалось загрузить файл: " + ex.Message);
                }
            }
        }

        //выбо предустановленного фильтра
        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            SetFilter(Core);
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            SetFilter(Core1);
        }

        private void toolStripButton3_Click(object sender, EventArgs e)
        {
            SetFilter(Core2);
        }

        private void toolStripButton11_Click(object sender, EventArgs e)
        {
            SetFilter(Core3);
        }

        private void toolStripButton10_Click(object sender, EventArgs e)
        {
            SetFilter(Core4);
        }

        private void toolStripButton9_Click(object sender, EventArgs e)
        {
            SetFilter(Core5);
        }

        private void toolStripButton8_Click(object sender, EventArgs e)
        {
            SetFilter(Core6);
        }

        private void toolStripButton7_Click(object sender, EventArgs e)
        {
            SetFilter(Core7);
        }

        private void toolStripButton6_Click(object sender, EventArgs e)
        {
            SetFilter(Core8);
        }

        private void toolStripButton5_Click(object sender, EventArgs e)
        {
            SetFilter(Core9);
        }
        //задание фильтра пользователем
        private void toolStripButton4_Click(object sender, EventArgs e)
        {
            InputCore UserInput = new InputCore();
            UserInput.ShowDialog();

            if (UserInput.CustemOk)
                SetFilter(UserInput.Core);
        }
    }
}

