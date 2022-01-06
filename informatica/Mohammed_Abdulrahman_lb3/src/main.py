class HouseScheme:

    def __init__(self, NumberOfRooms, Square, SharedBathroom):

        if (NumberOfRooms >= 0) and (Square > 0) and (type(SharedBathroom) == bool):

           self.NumberOfRooms = NumberOfRooms;

           self.Square = Square;

           self.SharedBathroom = SharedBathroom;

        else:

           raise ValueError("Invalid value")





class CountryHouse(HouseScheme): # Класс должен наследоваться от HouseScheme

    def __init__(self, NumberOfRooms, Square, SharedBathroom, NumberOfFloors, PlotSquare):

        super().__init__(NumberOfRooms, Square, SharedBathroom);

        if (NumberOfFloors > 0) and (PlotSquare > 0):

           self.NumberOfFloors = NumberOfFloors;

           self.PlotSquare = PlotSquare;

        else:

           raise ValueError("Invalid value")



    def __str__(self):

        return "Country House: Количество жилых комнат " + str(self.NumberOfRooms) + ", Жилая площадь " +str(self.Square) + ", Совмещенный санузел "+str(self.SharedBathroom) + ", Количество этажей " +str(self.NumberOfFloors) + ", Площадь участка " +str(self.PlotSquare) + "."    



    def __eq__(self, other):

        if (self.Square == other.Square) and (self.PlotSquare == other.PlotSquare) and (abs(self.NumberOfFloors - other.NumberOfFloors) <= 1):

            return True;

        else:

            return False;

        



class Apartment(HouseScheme): # Класс должен наследоваться от HouseScheme

    def __init__(self, NumberOfRooms, Square, SharedBathroom, Floor, View):

        super().__init__(NumberOfRooms, Square, SharedBathroom);

        if (1 <= Floor <= 15) and (View in ['N', 'S', 'W', 'E']):

           self.Floor = Floor;

           self.View = View

        else:

           raise ValueError("Invalid value");

    def __str__(self):

        return "Apartment: Количество жилых комнат " +str(self.NumberOfRooms) + ", Жилая площадь " +str(self.Square) + ", Совмещенный санузел " +str(self.SharedBathroom) + ", Этаж " +str(self.Floor) + ", Окна выходят на " +str(self.View) + "."



class CountryHouseList(list):

    def __init__(self, name):

       super().__init__();

       self.name = name;



    def append(self, p_object):

       if (type(p_object) == CountryHouse):

          super().append(p_object);

       else:

          raise TypeError("Invalid type " + str(type(p_object)));



    def total_square(self):

       result = 0;

       for element in self:

           result += element.Square;

       return result;



class ApartmentList(list):



      def __init__(self, name):

            super().__init__();

            self.name = name;



      def extend(self, iterable):

          for element in iterable:

              if type(element) == Apartment:

                  super().append(element)

      

      def floor_view(self, floors, directions):



          answer = list(filter(lambda apartment: apartment.Floor in range(floors[0], floors[1] + 1) and apartment.View in directions, self))

          for element in answer:

              print(element.View, ": ", element.Floor, sep='')

