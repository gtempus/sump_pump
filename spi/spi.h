/*
 * spi.h
 *
 * Created: 17.03.2015 12:59:35
 *  Author: mebskjoensfjell
 */ 


#ifndef SPI_H_
#define SPI_H_

#include "main.h"

/** SPI Mode */
#define SPI_MODE_0 0

/** SPI Mode */
struct spi_device {
	uint8_t id;
};

/** \brief Initialize SPI in master mode
 *
 * \param spi unused
 */
static inline void spi_master_init(void* spi)
{
	sysclk_enable_module(POWER_RED_REG0, PRSPI_bm);
	SPCR = (1 << MSTR);
}

/** \brief Configure SPI
 *
 * \param spi       unused
 * \param dev_conf  Slave config
 * \param mode      SPI mode
 * \param baud      Baud rate
 * \param foo       unused
 */
static inline void spi_master_setup_device(void *spi,  struct spi_device *dev_conf, uint8_t mode, uint32_t baud, uint8_t foo)
{
	SPCR |= (mode << CPHA);
	SPSR = (1 << SPI2X);	
}

/** \brief Enable SPI
 *
 * \param spi       unused
 */
static inline void spi_enable(void *spi)
{
	SPCR |= (1 << SPE);	
}

/** \brief Write data
 *
 * \param spi       unused
 * \param data      data to write
 */
static inline void spi_put(void *spi, uint8_t data)
{
	SPDR = data;
}

/** \brief Read data
 *
 * \param spi       unused
 * \param data      variable to store read data
 */
static inline void spi_get(void *spi, uint8_t *data)
{
	*data = SPDR;
}

/** \brief Select slave
 *
 * \param spi       unused
 * \param dev       slave
 */
static inline void spi_select_device(void *spi, struct spi_device *dev)
{
	ioport_set_pin_level(dev->id, false);
}

/** \brief Deselect slave
 *
 * \param spi       unused
 * \param dev       slave
 */
static inline void spi_deselect_device(void *spi, struct spi_device *dev)
{
	ioport_set_pin_level(dev->id, true);
}

status_code_t spi_write_packet(void *spi, uint8_t *data, uint8_t length);
status_code_t spi_read_single(void *spi, uint8_t *data);
#endif /* SPI_H_ */