#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "io.h"
#include "k/atapi.h"
#include "k/iso9660.h"

u16 main_reg;

// wait for sending packet
void busy_wait(u16 drive)
{
  u8 status;
  do {
    status = inb(ATA_REG_STATUS(drive));
  } while(status & BSY);
}

// waiting 4ms
void wait_device_selection()
{
  inb(PACKET);
  inb(PACKET);
  inb(PACKET);
  inb(PACKET);
}

// wait for BSY cleared and DRQ set
void wait_packet_request(u16 drive)
{
  u8 status;
  do {
    status = inb(ATA_REG_STATUS(drive));
  } while((status & BSY) || !(status & DRQ));
}

// select the drive
void select_drive(u16 reg, u8 slave)
{
  u16 dcr;
  if (reg == PRIMARY_REG)
    dcr = PRIMARY_DCR;
  else
    dcr = SECONDARY_DCR;

  outb(dcr, SRST);
  outb(dcr, INTERRUPT_DISABLE);
  outb(ATA_REG_DRIVE(reg), slave);
  wait_device_selection(reg);
}

// check if the drive is atapi drive by comparing signatures
bool is_atapi_drive(u16 bus, u8 drive)
{
  select_drive(bus, drive);
  u16 sig[4];
  sig[0] = inb(ATA_REG_SECTOR_COUNT(bus));
  sig[1] = inb(ATA_REG_LBA_LO(bus));
  sig[2] = inb(ATA_REG_LBA_MI(bus));
  sig[3] = inb(ATA_REG_LBA_HI(bus));
  u16 cmp[4];
  cmp[0] = ATAPI_SIG_SC;
  cmp[1] = ATAPI_SIG_LBA_LO;
  cmp[2] = ATAPI_SIG_LBA_MI;
  cmp[3] = ATAPI_SIG_LBA_HI;
  return !memcmp(sig, cmp, sizeof(sig));
}



void discover_atapi_drive()
{
  u16 drives[2] = {ATA_PORT_MASTER, ATA_PORT_SLAVE};
  u16 reg[2] = {PRIMARY_REG, SECONDARY_REG};

  for (u8 i = 0; i < 2; i++)
    for (u8 j = 0; j < 2; j++)
      if (is_atapi_drive(reg[i], drives[j]))
      {
        main_reg = reg[i];
        return;
      }
}

// method that process read data (TODO)
void process_data(u16 *data)
{
  struct iso_prim_voldesc *res;
  void *aux;
  char *str = (char*)data;
  str += ISO_BLOCK_SIZE*ISO_PRIM_VOLDESC_BLOCK;
  aux = str;
  res = aux;
  res = res; // remove wwarning
  //printf("%s\n", res->std_identifier);
}

u16 data[1024];
struct SCSI_packet pkt;

// main method that sends request for a packet then reads it
void send_packet()
{
  busy_wait(main_reg);

  outb(ATA_REG_FEATURES(main_reg), 0); /* No overlap/no DMA */
  outb(ATA_REG_SECTOR_COUNT(main_reg), 0); /* No queuing */
  outb(ATA_REG_LBA_MI(main_reg), CD_BLOCK_SZ & 0xff);
  outb(ATA_REG_LBA_HI(main_reg), CD_BLOCK_SZ >> 8);
  outb(ATA_REG_COMMAND(main_reg), 0xa0); /* PACKET */

  wait_packet_request(main_reg);

  // SCSI structure to be sent for a request
  int j = 17;
  pkt.op_code = READ_12;
  pkt.flags_lo = DPO;
  pkt.lba_hi = (j >> 24);
  pkt.lba_mihi = (j >> 16);
  pkt.lba_milo = (j >> 8);
  pkt.lba_lo = j;
  pkt.transfer_length_hi = 0;
  pkt.transfer_length_mihi = 0;
  pkt.transfer_length_milo = 0;
  pkt.transfer_length_lo = 1;

  // send packet request
  for (size_t i = 0; i < sizeof(pkt)/2; i++)
    outw(ATA_REG_DATA(main_reg), ((u16*)&pkt)[i]);

  u8 status;


  do {
    status = inb(ATA_REG_SECTOR_COUNT(main_reg));
  } while(status != PACKET_DATA_TRANSMIT);

  // reads data
  int i = 0;
  while ((status = inb(ATA_REG_SECTOR_COUNT(main_reg))) != PACKET_COMMAND_COMPLETE)
  {
    u16 aux = inw(ATA_REG_DATA(main_reg));
    data[i] = aux;
    i++;
  }


  //printf("%d\n", i);
  process_data(data);
}
