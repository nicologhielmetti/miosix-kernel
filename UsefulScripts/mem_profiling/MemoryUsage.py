class MemoryUsage:

    def __init__(self):
        self._stackUsedCurrent = None
        self._stackUsedMax = None
        self._stackFreeCurrent = None
        self._stackFreeMin = None
        self._heapUsedCurrent = None
        self._heapUsedMax = None
        self._heapFreeCurrent = None
        self._heapFreeMin = None

    @property
    def stackUsedCurrent(self):
        return self._stackUsedCurrent

    @stackUsedCurrent.setter
    def stackUsedCurrent(self, value):
        self._stackUsedCurrent = value

    @property
    def stackUsedMax(self):
        return self._stackUsedMax

    @stackUsedMax.setter
    def stackUsedMax(self, value):
        self._stackUsedMax = value

    @property
    def stackFreeCurrent(self):
        return self._stackFreeCurrent

    @stackFreeCurrent.setter
    def stackFreeCurrent(self, value):
        self._stackFreeCurrent = value

    @property
    def stackFreeMin(self):
        return self._stackFreeMin

    @stackFreeMin.setter
    def stackFreeMin(self, value):
        self._stackFreeMin = value

    @property
    def heapUsedCurrent(self):
        return self._heapUsedCurrent

    @heapUsedCurrent.setter
    def heapUsedCurrent(self, value):
        self._heapUsedCurrent = value

    @property
    def heapUsedMax(self):
        return self._heapUsedMax

    @heapUsedMax.setter
    def heapUsedMax(self, value):
        self._heapUsedMax = value

    @property
    def heapFreeCurrent(self):
        return self._heapFreeCurrent

    @heapFreeCurrent.setter
    def heapFreeCurrent(self, value):
        self._heapFreeCurrent = value

    @property
    def heapFreeMin(self):
        return self._heapFreeMin

    @heapFreeMin.setter
    def heapFreeMin(self, value):
        self._heapFreeMin = value

    def toCSVLine(self):
        return str(self.stackUsedCurrent) + ',' + \
               str(self.stackUsedMax) + ',' + \
               str(self.stackFreeCurrent) + ',' + \
               str(self.stackFreeMin) + ',' + \
               str(self.heapUsedCurrent) + ',' + \
               str(self.heapUsedMax) + ',' + \
               str(self.heapFreeCurrent) + ',' + \
               str(self.heapFreeMin)