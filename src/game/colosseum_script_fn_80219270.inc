    nofralloc
    rlwinm r4, r3, 0, 16, 31
    cmplwi r4, 0x164
    beq @L_802192A4
    subi r0, r3, 0xa5
    rlwinm r0, r0, 0, 16, 31
    cmplwi r0, 0x1
    ble @L_802192A4
    cmplwi r4, 0xffff
    beq @L_802192A4
    cmplwi r4, 0x0
    beq @L_802192A4
    cmplwi r4, 0x165
    bne @L_802192AC
@L_802192A4:
    li r3, 0x1
    blr
@L_802192AC:
    li r3, 0x0
    blr
