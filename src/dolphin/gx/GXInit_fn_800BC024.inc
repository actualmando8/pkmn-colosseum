    nofralloc
    lwz r3, -29632(r13)
    li r6, 0x0
    li r4, 0x0
    lwz r0, 0x204(r3)
    rlwinm r0, r0, 16, 29, 31
    cmplwi r0, 0x0
    mtctr r0
    ble @L_800BC0AC
@L_800BC044:
    cmpwi r4, 0x2
    beq @L_800BC084
    bge @L_800BC060
    cmpwi r4, 0x0
    beq @L_800BC06C
    bge @L_800BC078
    b @L_800BC098
@L_800BC060:
    cmpwi r4, 0x4
    bge @L_800BC098
    b @L_800BC090
@L_800BC06C:
    lwz r0, 0x120(r3)
    rlwinm r5, r0, 0, 29, 31
    b @L_800BC098
@L_800BC078:
    lwz r0, 0x120(r3)
    rlwinm r5, r0, 26, 29, 31
    b @L_800BC098
@L_800BC084:
    lwz r0, 0x120(r3)
    rlwinm r5, r0, 20, 29, 31
    b @L_800BC098
@L_800BC090:
    lwz r0, 0x120(r3)
    rlwinm r5, r0, 14, 29, 31
@L_800BC098:
    li r0, 0x1
    slw r0, r0, r5
    or r6, r6, r0
    addi r4, r4, 0x1
    bdnz @L_800BC044
@L_800BC0AC:
    addi r4, r3, 0x124
    lwz r3, 0x124(r3)
    rlwinm r0, r3, 0, 24, 31
    cmplw r0, r6
    beqlr
    rlwinm r0, r3, 0, 0, 23
    or r0, r0, r6
    stw r0, 0x0(r4)
    li r0, 0x61
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    li r0, 0x0
    lwz r4, -29632(r13)
    lwz r3, 0x124(r4)
    stw r3, -0x8000(r5)
    sth r0, 0x2(r4)
    blr
