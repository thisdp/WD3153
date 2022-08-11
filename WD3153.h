#define REG_RST		0x00	//Chip ID and Software Reset Register RSTR 
#define REG_GC		0x01	//Global Control Register GCR
#define REG_IS		0x02
#define REG_FCT 	0x03
#define REG_LCT 	0x30
#define REG_LCFG0	0x31
#define REG_LCFG1	0x32
#define REG_LCFG2	0x33
#define REG_PWM0	0x34
#define REG_PWM1	0x35
#define REG_PWM2	0x36
#define REG_LED0T1	0x37
#define REG_LED0T3	0x38
#define REG_LED0T0	0x39
#define REG_LED1T1	0x3A
#define REG_LED1T3	0x3B
#define REG_LED1T0	0x3C
#define REG_LED2T1	0x3D
#define REG_LED2T3	0x3E
#define REG_LED2T0	0x3F
#define REG_LEDSYN	0x4A
#define REG_IAD		0x77

#define LED0 0b00000000
#define LED1 0b00000001
#define LED2 0b00000010

//Wire.begin() need to be called in setup!
class WD3153{
public:
	WD3153(){
		address = 0x45;
		chipID = 0x33;
	}
	void setChipID(uint8_t chipID){
		writeReg(REG_RST,chipID);
	}
	uint8_t getChipID(uint8_t chipID){
		return readReg(REG_RST);
	}
	
	void reset(){
		writeReg(REG_RST,0x55);
	}
	
	void setChipEnabled(bool state){
		uint8_t gc = readReg(REG_GC);
		writeReg(REG_GC,gc&0b11111110|state);
	}
	bool isChipEnabled(){
		uint8_t gc = readReg(REG_GC);
		return gc&0x01;
	}
	
	void setUVLOInterruptEnabled(bool state){
		uint8_t gc = readReg(REG_GC);
		writeReg(REG_GC,gc&0b11101111|(state<<4));
	}
	bool isUVLOInterruptEnabled(){
		uint8_t gc = readReg(REG_GC);
		return (gc>>4)&0x01;
	}
	
	void setOTPInterruptEnabled(bool state){
		uint8_t gc = readReg(REG_GC);
		writeReg(REG_GC,gc&0b11110111|(state<<3));
	}
	bool isOTPInterruptEnabled(){
		uint8_t gc = readReg(REG_GC);
		return (gc>>3)&0x01;
	}
	
	void setLEDInterruptEnabled(uint8_t ledX,bool state){
		if(ledX>2) return;
		uint8_t gc = readReg(REG_GC);
		writeReg(REG_GC,gc&(~(1<<(ledX+5)))|(state<<(ledX+5)));
	}
	bool isLEDInterruptEnabled(uint8_t ledX,bool state){
		if(ledX>2) return false;
		uint8_t gc = readReg(REG_GC);
		return (gc>>(ledX+5))&0x01;
	}
	//Interrupt Status
	//Power On Reset Interrupt
	void setPORInterruptStatus(bool state){
		uint8_t is = readReg(REG_IS);
		writeReg(REG_GC,is&0b11101111|(state<<4));
	}
	bool getPORInterruptStatus(){
		uint8_t is = readReg(REG_IS);
		return (is>>4)&0x1;
	}
	
	//Under Voltage Lock Out Interrupt
	bool getUVLOInterruptStatus(){
		uint8_t is = readReg(REG_IS);
		return (is>>3)&0x1;
	}
	
	//Over Temperature Interrupt
	bool getOTPInterruptStatus(){
		uint8_t is = readReg(REG_IS);
		return (is>>2)&0x1;
	}
	
	void setLEDInterruptStatus(uint8_t ledX,bool state){
		if(ledX>2) return;
		uint8_t is = readReg(REG_IS);
		writeReg(REG_IS,is&(~(1<<(ledX+5)))|(state<<(ledX+5)));
	}
	bool getLEDInterruptStatus(uint8_t ledX){
		if(ledX>2) return false;
		uint8_t is = readReg(REG_IS);
		return (is>>(ledX+5))&0x01;
	}
	
	//Function Control
	void setOTPEnabled(bool state){
		uint8_t fc = readReg(REG_FCT);
		writeReg(REG_FCT,fc&0b11101111|(state<<4));
	}
	bool isOTPEnabled(){
		uint8_t fc = readReg(REG_FCT);
		return (fc>>4)&0x01;
	}
	
	void setUVLOEnabled(bool state){
		uint8_t fc = readReg(REG_FCT);
		writeReg(REG_FCT,fc&0b11110111|(state<<3));
	}
	bool isUVLOEnabled(){
		uint8_t fc = readReg(REG_FCT);
		return (fc>>3)&0x01;
	}
	
	void setCHRGEnabled(bool state){
		uint8_t fc = readReg(REG_FCT);
		writeReg(REG_FCT,fc&0b11111011|((!state)<<2));
	}
	bool isCHRGEnabled(){
		uint8_t fc = readReg(REG_FCT);
		return (~(fc>>2))&0x01;
	}
		
	#define PWM250HZ 0
	#define PWM500HZ 1
	void setGlobalPWMFrequencyLevel(bool state){
		uint8_t fc = readReg(REG_FCT);
		writeReg(REG_FCT,fc&0b11111101|(state<<1));
	}
	bool getGlobalPWMFrequencyLevel(){
		uint8_t fc = readReg(REG_FCT);
		return (fc>>1)&0x01;
	}
	
	#define EasingLinear 0
	#define EasingLog 1
	void setGlobalPWMEasingFunction(bool state){
		uint8_t fc = readReg(REG_FCT);
		writeReg(REG_FCT,fc&0b11111110|state);
	}
	bool getGlobalPWMEasingFunction(){
		uint8_t fc = readReg(REG_FCT);
		return fc&0x01;
	}
	
	//LED Enabled
	void setLEDEnabled(uint8_t ledX,bool state){
		if(ledX>2) return;
		uint8_t lct = readReg(REG_LCT);
		writeReg(REG_LCT,lct&(~(1<<ledX))|(state<<ledX));
	}
	bool isLEDEnabled(uint8_t ledX){
		if(ledX>2) return false;
		uint8_t lct = readReg(REG_LCT);
		return (lct>>ledX)&0x01;
	}
	
	//Fade In/Out Mode
	//Fade Out uses T3
	//Fade In uses T1
	void setLEDFadeOutEnabled(uint8_t ledX, bool state){
		if(ledX>2) return;
		uint8_t lcfg = readReg(REG_LCFG0+ledX);
		writeReg(REG_LCFG0+ledX,lcfg&0b10111111|(state<<6));
	}
	bool isLEDFadeOutEnabled(uint8_t ledX){
		if(ledX>2) return false;
		uint8_t lcfg = readReg(REG_LCFG0+ledX);
		return (lcfg>>6)&0x01;
	}
	
	void setLEDFadeInEnabled(uint8_t ledX, bool state){
		if(ledX>2) return;
		uint8_t lcfg = readReg(REG_LCFG0+ledX);
		writeReg(REG_LCFG0+ledX,lcfg&0b11011111|(state<<5));
	}
	bool isLEDFadeInEnabled(uint8_t ledX){
		if(ledX>2) return false;
		uint8_t lcfg = readReg(REG_LCFG0+ledX);
		return (lcfg>>5)&0x01;
	}
	
	#define DirectControlMode 0
	#define ProgrammableLightingMode 1
	void setLEDMode(uint8_t ledX, bool mode){
		if(ledX>2) return;
		uint8_t lcfg = readReg(REG_LCFG0+ledX);
		writeReg(REG_LCFG0+ledX,lcfg&0b11101111|(mode<<4));
	}
	bool getLEDMode(uint8_t ledX){
		if(ledX>2) return false;
		uint8_t lcfg = readReg(REG_LCFG0+ledX);
		return (lcfg>>4)&0x01;
	}
	
	#define LED0mA 0b00000000
	#define LED5mA 0b00000001
	#define LED10mA 0b00000010
	#define LED15mA 0b00000011
	#define LED25mA 0b00000100
	void setLEDCurrentLimit(uint8_t ledX, uint8_t currentLimit){
		if(ledX>2) return;
		uint8_t lcfg = readReg(REG_LCFG0+ledX);
		writeReg(REG_LCFG0+ledX,lcfg&0b11111000|currentLimit);
	}
	uint8_t getLEDCurrentLimit(uint8_t ledX){
		if(ledX>2) return false;
		uint8_t lcfg = readReg(REG_LCFG0+ledX);
		return lcfg&0b00000111;
	}
	
	//PWM
	void setLEDPWM(uint8_t ledX,uint8_t duty){
		writeReg(REG_PWM0+ledX,duty);
	}
	uint8_t getLEDPWM(uint8_t ledX){
		return readReg(REG_PWM0+ledX);
	}
	
	//T0 T1 T2 T3 T4
	#define T0 0
	#define T1 1
	#define T2 2
	#define T3 3
	#define T4 4
	
	#define T0s 0b00000000
	#define T0_13s 0b00000001
	#define T0_26s 0b00000010
	#define T0_52s 0b00000011
	#define T1_04s 0b00000100
	#define T2_08s 0b00000101
	#define T4_16s 0b00000110
	#define T8_32s 0b00000111
	#define T16_64s 0b00001000
	void setLEDTime(uint8_t ledX, uint8_t timeX, uint8_t timeValue){
		if(ledX>2 || timeValue>0b00001000) return false;
		uint8_t ledT;
		switch(timeX){
			case T0: //Only T0 supports 0s
				ledT = readReg(REG_LED0T0+ledX*3);
				writeReg(REG_LED0T0+ledX*3,ledT&0b00001111|(timeValue<<4));
				break;
			case T1: //T1 16.64s max
				if(timeValue == T0s) return;
				ledT = readReg(REG_LED0T1+ledX*3);
				writeReg(REG_LED0T1+ledX*3,ledT&0b10001111|((timeValue-1)<<4));
				break;
			case T2: //T2 4.15s max
				if(timeValue == T0s) return;
				ledT = readReg(REG_LED0T1+ledX*3);
				writeReg(REG_LED0T1+ledX*3,ledT&0b11111000|(timeValue-1));
				break;
			case T3: //T3 16.64s max
				if(timeValue == T0s) return;
				ledT = readReg(REG_LED0T3+ledX*3);
				writeReg(REG_LED0T3+ledX*3,ledT&0b10001111|((timeValue-1)<<4));
				break;
			case T4: //T4 16.64s max
				if(timeValue == T0s) return;
				ledT = readReg(REG_LED0T3+ledX*3);
				writeReg(REG_LED0T3+ledX*3,ledT&0b11111000|(timeValue-1));
				break;
		}
	}
	uint8_t getLEDTime(uint8_t ledX, uint8_t timeX){
		if(ledX>2) return false;
		uint8_t ledT;
		switch(timeX){
			case T0:
				ledT = readReg(REG_LED0T0+ledX*3);
				return (ledT>>4)&0b00001111;
			case T1:
				ledT = readReg(REG_LED0T1+ledX*3);
				return (ledT>>4)&0b00000111 +1;
			case T2:
				ledT = readReg(REG_LED0T1+ledX*3);
				return ledT&0b00000111 +1;
			case T3:
				ledT = readReg(REG_LED0T3+ledX*3);
				return (ledT>>4)&0b00000111 +1;
			case T4:
				ledT = readReg(REG_LED0T3+ledX*3);
				return ledT&0b00000111 +1;
		}
	}
	
	//Blink repeat times 1~15, 0 For infinity
	void setLEDBlinkTimes(uint8_t ledX,uint8_t repeatTimes){
		if(ledX>2 || repeatTimes >15) return;
		uint8_t ledT = readReg(REG_LED0T0+ledX*3);
		writeReg(REG_LED0T0+ledX*3,ledT&0b00001111|repeatTimes);
	}
	uint8_t getLEDBlinkTimes(uint8_t ledX){
		if(ledX>2) return;
		uint8_t ledT = readReg(REG_LED0T0+ledX*3);
		return ledT&0b00001111;
	}
	
	//Sync
	void setSyncEnabled(bool state){
		uint8_t ledSync = readReg(REG_LEDSYN);
		writeReg(REG_LEDSYN,ledSync&0b00000100|(state<<2));
	}
	bool isSyncEnabled(){
		uint8_t ledSync = readReg(REG_LEDSYN);
		return (ledSync>>2)&0x01;
	}
	
	//I2C Address
	void setI2CAddress(uint8_t addr){
		if(addr>0b01111111) return; //invalid
		writeReg(REG_IAD,0b10000000|addr);
		address = addr;
	}
	uint8_t getI2CAddress(){
		return address;
	}
	
private:
	inline uint8_t readReg(uint8_t reg){
		Wire.beginTransmission(address);
		Wire.write(reg);
		Wire.endTransmission();
		Wire.requestFrom(address,(uint8_t)1);
		while (Wire.available())
			return Wire.read();
	}
	inline void writeReg(uint8_t reg, uint8_t data){
		Wire.beginTransmission(address);
		Wire.write(reg);
		Wire.write(data);
		Wire.endTransmission();
	}
	uint8_t address;
	uint8_t chipID;
};