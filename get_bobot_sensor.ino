void get_bobot_mpux(double val_data_set, int baris) {
  double y, m, c;
  double awal, puncak, akhir;
  double konstanta = 0.1;
  double iterasi;
  iterasi = max_mpux / konstanta;
  double nilai_iterasi;
  for (int j = 0; j < baris - 1; j++) {
    bobot_mpux[j] = 0;
  }
  for (int j = 0; j <= baris - 1; j++) {
    awal = mf_sen_mpux[j][0];
    puncak = mf_sen_mpux[j][1];
    akhir = mf_sen_mpux[j][2];
    nilai_iterasi = 0;
    for (int i = 0; i <= iterasi ; i++) {
      nilai_iterasi = nilai_iterasi + konstanta;
      if (nilai_iterasi < awal) {
        if (j == 0) {
          y = 1;
        } else {
          y = 0;
        }
      } else if (nilai_iterasi >= awal && nilai_iterasi < puncak) {
        if (j == 0) {
          y = 1;
        } else {
          m = (puncak - awal);
          m = 1 / m;
          c = (m * puncak);
          c = 1 - c;
          y = (m * nilai_iterasi);
          y = y + c;
        }
      } else if (nilai_iterasi >= puncak && nilai_iterasi < akhir) {
        if (j == baris - 1) {
          y = 1;
        } else {
          m = (puncak - akhir);
          m = 1 / m;
          c = (m * puncak);
          c = 1 - c;
          y = (m * nilai_iterasi);
          y = y + c;
        }
      } else if (nilai_iterasi >= akhir) {
        if (j == baris - 1) {
          y = 1;
        } else {
          y = 0;
        }
      }
      if (nilai_iterasi >= val_data_set) {
        bobot_mpux[j] = y;
        Serial.print("Member Ship mpux: ");
        Serial.print(j);
        Serial.print(" : ");
        Serial.println(bobot_mpux[j]);
        break;
      }
    }
  }
  Serial.println("==========================");
}




void get_bobot_mpuy(double val_data_set, int baris) {
  double y, m, c;
  double awal, puncak, akhir;
  double konstanta = 0.1;
  double iterasi;
  iterasi = max_mpuy / konstanta;
  double nilai_iterasi;

  for (int j = 0; j < baris - 1; j++) {
    bobot_mpuy[j] = 0;
  }

  for (int j = 0; j <= baris - 1; j++) {
    awal = mf_sen_mpuy[j][0];
    puncak = mf_sen_mpuy[j][1];
    akhir = mf_sen_mpuy[j][2];
    nilai_iterasi = 0;

    for (int i = 0; i <= iterasi ; i++) {
      nilai_iterasi = nilai_iterasi + konstanta;

      if (nilai_iterasi < awal) {
        if (j == 0) {
          y = 1;
        } else {
          y = 0;
        }
      } else if (nilai_iterasi >= awal && nilai_iterasi < puncak) {
        if (j == 0) {
          y = 1;
        } else {
          m = (puncak - awal);
          m = 1 / m;
          c = (m * puncak);
          c = 1 - c;
          y = (m * nilai_iterasi);
          y = y + c;
        }
      } else if (nilai_iterasi >= puncak && nilai_iterasi < akhir) {
        if (j == baris - 1) {
          y = 1;
        } else {
          m = (puncak - akhir);
          m = 1 / m;
          c = (m * puncak);
          c = 1 - c;
          y = (m * nilai_iterasi);
          y = y + c;
        }
      } else if (nilai_iterasi >= akhir) {
        if (j == baris - 1) {
          y = 1;
        } else {
          y = 0;
        }
      }

      if (nilai_iterasi >= val_data_set) {
        bobot_mpuy[j] = y;
        Serial.print("Member Ship mpuy: ");
        Serial.print(j);
        Serial.print(" : ");
        Serial.println(bobot_mpuy[j]);
        break;
      }
    }
  }
  Serial.println("==========================");
}


void get_bobot_distance(double val_data_set, int baris) {
  double y, m, c;
  double awal, puncak, akhir;
  double konstanta = 0.1;
  double iterasi;
  iterasi = max_distance / konstanta;
  double nilai_iterasi;

  for (int j = 0; j < baris-1 ; j++) {
    bobot_distance[j] = 0;
  }

  for (int j = 0; j <= baris - 1; j++) {
    awal = mf_sen_distance[j][0];
    puncak = mf_sen_distance[j][1];
    akhir = mf_sen_distance[j][2];
    nilai_iterasi = 0;

    for (int i = 0; i <= iterasi ; i++) {
      nilai_iterasi = nilai_iterasi + konstanta;

      if (nilai_iterasi < awal) {
        if (j == 0) {
          y = 1;
        } else {
          y = 0;
        }
      } else if (nilai_iterasi >= awal && nilai_iterasi < puncak) {
        if (j == 0) {
          y = 1;
        } else {
          m = (puncak - awal);
          m = 1 / m;
          c = (m * puncak);
          c = 1 - c;
          y = (m * nilai_iterasi);
          y = y + c;
        }
      } else if (nilai_iterasi >= puncak && nilai_iterasi < akhir) {
        if (j == baris - 1) {
          y = 1;
        } else {
          m = (puncak - akhir);
          m = 1 / m;
          c = (m * puncak);
          c = 1 - c;
          y = (m * nilai_iterasi);
          y = y + c;
        }
      } else if (nilai_iterasi >= akhir) {
        if (j == baris - 1) {
          y = 1;
        } else {
          y = 0;
        }
      }

      if (nilai_iterasi >= val_data_set) {
        bobot_distance[j] = y;
        Serial.print("Member Ship distance: ");
        Serial.print(j);
        Serial.print(" : ");
        Serial.println(bobot_distance[j]);
        break;
      }
    }
  }
  Serial.println("==========================");
}


void get_bobot_rh(double val_data_set, int baris) {
  double y, m, c;
  double awal, puncak, akhir;
  double konstanta = 0.11;
  double iterasi;
  iterasi = max_rh / konstanta;
  double nilai_iterasi;

  for (int j = 0; j < baris-1 ; j++) {
    bobot_rh[j] = 0;
  }

  for (int j = 0; j <= baris - 1; j++) {
    awal = mf_sen_rh[j][0];
    puncak = mf_sen_rh[j][1];
    akhir = mf_sen_rh[j][2];
    nilai_iterasi = 0;

    for (int i = 0; i <= iterasi ; i++) {
      nilai_iterasi = nilai_iterasi + konstanta;

      if (nilai_iterasi < awal) {
        if (j == 0) {
          y = 1;
        } else {
          y = 0;
        }
      } else if (nilai_iterasi >= awal && nilai_iterasi < puncak) {
        if (j == 0) {
          y = 1;
        } else {
          m = (puncak - awal);
          m = 1 / m;
          c = (m * puncak);
          c = 1 - c;
          y = (m * nilai_iterasi);
          y = y + c;
        }
      } else if (nilai_iterasi >= puncak && nilai_iterasi < akhir) {
        if (j == baris - 1) {
          y = 1;
        } else {
          m = (puncak - akhir);
          m = 1 / m;
          c = (m * puncak);
          c = 1 - c;
          y = (m * nilai_iterasi);
          y = y + c;
        }
      } else if (nilai_iterasi >= akhir) {
        if (j == baris - 1) {
          y = 1;
        } else {
          y = 0;
        }
      }

      if (nilai_iterasi >= val_data_set) {
        bobot_rh[j] = y;
        Serial.print("Member Ship rh: ");
        Serial.print(j);
        Serial.print(" : ");
        Serial.println(bobot_rh[j]);
        break;
      }
    }
  }
  Serial.println("==========================");
}
