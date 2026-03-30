    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    li r31, 0x0
    li r3, 0x1
    bl fn_800F7BC4
    rlwinm r0, r3, 0, 28, 31
    cmplwi r0, 0x0
    beq @L_801CB2E4
    li r31, 0x1
@L_801CB2E4:
    li r3, 0x1
    li r4, 0x0
    bl fn_800F7A7C
    extsb r0, r3
    cmpwi r0, 0x0
    ble @L_801CB310
    li r3, 0x1
    li r4, 0x0
    bl fn_800F7A7C
    extsb r0, r3
    b @L_801CB324
@L_801CB310:
    li r3, 0x1
    li r4, 0x0
    bl fn_800F7A7C
    extsb r0, r3
    neg r0, r0
@L_801CB324:
    cmpwi r0, 0x2
    ble @L_801CB330
    li r31, 0x1
@L_801CB330:
    li r3, 0x1
    li r4, 0x0
    bl fn_800F7A08
    extsb r0, r3
    cmpwi r0, 0x0
    ble @L_801CB35C
    li r3, 0x1
    li r4, 0x0
    bl fn_800F7A08
    extsb r0, r3
    b @L_801CB370
@L_801CB35C:
    li r3, 0x1
    li r4, 0x0
    bl fn_800F7A08
    extsb r0, r3
    neg r0, r0
@L_801CB370:
    cmpwi r0, 0x2
    ble @L_801CB37C
    li r31, 0x1
@L_801CB37C:
    mr r3, r31
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
