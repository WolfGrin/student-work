/*
    Приложение выполняет линейную фильтрацию изображения.
    Программа содержит предустановленный комплект популярных фильтров,
    так-же позволяет применять пользовательские фильтры используя ядро свертки размерностью 3х3
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LineFilter
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());

            // Завершить приложение 
            Application.Exit();
        }


    }


}
