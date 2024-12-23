class Square:
    def __init__(self, x, y):
        self.__x = x
        self.__y = y

    def getX(self):
        return self.__x

    def getY(self):
        return self.__y

    def __str__(self):
        return str(self.__x) + " " + str(self.__y)
