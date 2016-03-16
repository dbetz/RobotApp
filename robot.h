class Robot {
public:
    Robot();
    ~Robot();
    bool processButton(int whichWay);
    void setSpeed(int left, int right);
private:
    int m_wheelLeft;
    int m_wheelRight;
};
