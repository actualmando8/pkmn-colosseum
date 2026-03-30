    nofralloc
    rlwinm. r0, r3, 0, 28, 28
    rlwinm r4, r3, 0, 24, 31
    rlwinm r5, r3, 0, 0, 23
    beq @L_800D0E10
    li r3, 0x8
    blr
@L_800D0E10:
    andi. r0, r4, 0x47
    beq @L_800D0E20
    li r3, 0x40
    blr
@L_800D0E20:
    cmplwi r4, 0x0
    beq @L_800D0E30
    li r3, 0x80
    blr
@L_800D0E30:
    rlwinm. r3, r5, 0, 3, 4
    bne @L_800D0E98
    rlwinm r3, r5, 0, 0, 15
    lis r0, 0x4
    cmpw r3, r0
    beqlr
    bge @L_800D0E6C
    lis r0, 0x2
    cmpw r3, r0
    beqlr
    bge @L_800D0E90
    lis r0, 0x1
    cmpw r3, r0
    beqlr
    b @L_800D0E90
@L_800D0E6C:
    lis r0, 0x500
    cmpw r3, r0
    beqlr
    bge @L_800D0E90
    lis r0, 0x200
    cmpw r3, r0
    beqlr
    b @L_800D0E90
    blr
@L_800D0E90:
    li r3, 0x40
    blr
@L_800D0E98:
    subis r0, r3, 0x800
    cmplwi r0, 0x0
    beq @L_800D0EAC
    li r3, 0x40
    blr
@L_800D0EAC:
    rlwinm r3, r5, 0, 0, 15
    lis r0, 0x900
    cmpw r3, r0
    beqlr
    bge @L_800D0ED4
    lis r0, 0x800
    cmpw r3, r0
    beqlr
    b @L_800D0ED4
    blr
@L_800D0ED4:
    rlwinm r3, r5, 0, 0, 10
    subis r0, r3, 0x820
    cmplwi r0, 0x0
    bne @L_800D0EEC
    lis r3, 0x820
    blr
@L_800D0EEC:
    rlwinm. r0, r5, 0, 0, 0
    beq @L_800D0F24
    rlwinm. r0, r5, 0, 5, 5
    bne @L_800D0F24
    andis. r3, r5, 0x8b10
    addis r0, r3, 0x74f0
    cmplwi r0, 0x0
    bne @L_800D0F14
    lis r3, 0x8b10
    blr
@L_800D0F14:
    rlwinm. r0, r5, 0, 6, 6
    bne @L_800D0F24
    lis r3, 0x8800
    blr
@L_800D0F24:
    andis. r3, r5, 0x900
    subis r0, r3, 0x900
    cmplwi r0, 0x0
    bne @L_800D0F3C
    lis r3, 0x900
    blr
@L_800D0F3C:
    li r3, 0x40
    blr
