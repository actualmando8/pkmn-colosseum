    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    li r31, 0x0
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    lwz r0, 0x1c(r3)
    rlwinm r0, r0, 0, 31, 31
    cmpwi r0, 0x1
    bne @L_801D98A8
    lwz r4, 0x8(r29)
    cmpwi r4, 0x0
    ble @L_801D9930
    lwz r3, 0xb0(r29)
    bl fn_801DCDA8
    mr r30, r3
    lwz r0, 0x70(r29)
    lwz r3, 0x70(r3)
    cmpw r3, r0
    ble @L_801D9858
    lis r3, -32728
    addi r3, r3, -26880
    crxor 6, 6, 6
    bl fn_800DD970
@L_801D9858:
    lwz r0, 0x4(r30)
    cmpwi r0, 0x2
    bne @L_801D98A0
    lwz r3, 0xa4(r30)
    cmplwi r3, 0x0
    beq @L_801D9930
    lwz r4, 0x24(r29)
    bl fn_800EE150
    mr r30, r3
    bl fn_800057A8
    cmpwi r3, 0x2
    bne @L_801D9898
    cmplwi r30, 0x0
    bne @L_801D9898
    li r3, 0x2
    bl fn_801D744C
@L_801D9898:
    mr r3, r30
    b @L_801D9934
@L_801D98A0:
    li r3, 0x0
    b @L_801D9934
@L_801D98A8:
    lwz r3, 0xb0(r29)
    lwz r4, 0x20(r29)
    lwz r30, 0x3c(r3)
    cmpwi r4, 0x10
    lhz r0, 0x32(r30)
    lwz r3, 0x2c(r30)
    mulli r0, r0, 0xd4
    add r29, r3, r0
    bne @L_801D98F4
    lwz r3, 0x24(r30)
    bl fn_800E6BC8
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801D98EC
    lwz r3, 0x24(r30)
    bl fn_800EE0E8
    subi r4, r3, 0x1
    b @L_801D9900
@L_801D98EC:
    lwz r4, 0x54(r29)
    b @L_801D9900
@L_801D98F4:
    rlwinm r0, r4, 2, 0, 29
    add r3, r29, r0
    lwz r4, 0x4c(r3)
@L_801D9900:
    cmpwi r4, 0x0
    blt @L_801D9914
    lwz r3, 0x24(r30)
    bl fn_800EE150
    mr r31, r3
@L_801D9914:
    bl fn_800057A8
    cmpwi r3, 0x2
    bne @L_801D9930
    cmplwi r31, 0x0
    bne @L_801D9930
    li r3, 0x2
    bl fn_801D744C
@L_801D9930:
    mr r3, r31
@L_801D9934:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
