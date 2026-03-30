    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr. r29, r3
    beq @L_801DA2A8
    rlwinm r0, r30, 0, 29, 29
    rlwinm r31, r30, 0, 24, 31
    cmpwi r0, 0x4
    bne @L_801DA260
    bl fn_801DD078
    b @L_801DA264
@L_801DA260:
    bl fn_801DD028
@L_801DA264:
    rlwinm r0, r31, 0, 28, 28
    cmpwi r0, 0x8
    bne @L_801DA27C
    mr r3, r29
    bl fn_801DCFD8
    b @L_801DA284
@L_801DA27C:
    mr r3, r29
    bl fn_801DCF84
@L_801DA284:
    rlwinm r0, r31, 0, 30, 30
    cmpwi r0, 0x2
    bne @L_801DA29C
    mr r3, r29
    bl fn_801DCF00
    b @L_801DA2A4
@L_801DA29C:
    mr r3, r29
    bl fn_801DCEA8
@L_801DA2A4:
    stb r30, 0x18(r29)
@L_801DA2A8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
