#include "sine_cos_table.h"
int alpha;



// PLS MW_TGM
struct mw_tgm_struct {
  uint16_t amw = 180;
  uint16_t dist[180] = {77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 82, 82, 77, 82, 82, 82, 82, 82, 82, 82, 82, 82, 87, 87, 87, 87, 87, 87, 92, 87, 92, 92, 92, 97, 97, 97, 97, 102, 102, 107, 102, 107, 107, 112, 112, 112, 117, 122, 122, 122, 127, 132, 132, 132, 142, 142, 147, 152, 157, 162, 162, 167, 177, 187, 192, 197, 207, 217, 227, 232, 244, 259, 274, 289, 309, 329, 354, 379, 414, 454, 499, 839, 839, 834, 849, 1039, 1319, 1759, 5109, 1764, 1764, 1724, 1714, 1289, 1044, 874, 839, 674, 654, 654, 494, 449, 419, 419, 419, 344, 314, 299, 279, 269, 259, 249, 234, 224, 214, 209, 199, 194, 184, 179, 174, 169, 164, 159, 154, 154, 149, 144, 144, 144, 137, 137, 132, 127, 132, 124, 122, 119, 119, 117, 117, 117, 112, 112, 107, 112, 107, 102, 107, 102, 102, 97, 102, 102, 97, 102, 97, 97, 97, 97, 92, 92, 92, 92, 92, 89, 92, 92, 92, 87, 87, 87, 92, 87, 87, 87, 87, 87, 87, 87};
  //uint16_t data[180];
  //uint8_t tgm_status;
  //const String dist_unit = "cm";
  //bool glare_flag[180]; // not smart to use bool because bool -> uint8_t
  //bool wf_v_flag[180];  // not smart to use bool because bool -> uint8_t
  //bool pf_v_flag[180];  // not smart to use bool because bool -> uint8_t
};
mw_tgm_struct mw_tgm;

// PLS measurements cartesian
struct mw_cart_struct {
  uint16_t amw;
  int16_t x[180]; // in cm
  int16_t y[180]; // in cm
};
mw_cart_struct mw_cart;

void setup() {
  Serial.begin(115200);


  PLSpolar2cart(&mw_tgm, &mw_cart);


  Serial.print("PLS_x = [");
  for (uint8_t n = 0; n < mw_cart.amw; n++)
  {
    Serial.print(mw_cart.x[n]);
    Serial.print(", ");
  }
  Serial.println("];");

  Serial.print("PLS_y = [");
  for (uint8_t n = 0; n < mw_cart.amw; n++)
  {
    Serial.print(mw_cart.y[n]);
    Serial.print(", ");
  }
  Serial.println("];");


}

void loop() {

}

void PLSpolar2cart(mw_tgm_struct* polar, mw_cart_struct* cart)
{
  int32_t temp_value;

  cart->amw = polar->amw;
  for (uint8_t n = 0; n < polar->amw; n++)
  {
    if (n == 0) Serial.println((int32_t)polar->dist[n]);
    if (n == 0) Serial.println((int32_t)cos_tab[n]);

    temp_value = (int32_t)77 * (int32_t)4096;
    if (n == 0) Serial.println(temp_value);
    temp_value = (int32_t)polar->dist[n] * (int32_t)cos_tab[n];
    if (n == 0) Serial.println(temp_value);


    /*temp_value = temp_value / COS_NORM;
      cart->x[n] =  (int16_t)temp_value;

      temp_value = (int32_t)polar->dist[n] * sine_tab[n];
      temp_value = temp_value / SINE_NORM;
      cart->y[n] =  (int16_t)temp_value;*/
  }

}

