    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r3, r4
    li r4, 0x2e
    stw r0, 0x14(r1)
    bl fn_80202360
    extsh r0, r3
    cmpwi r0, 0x3
    beq @L_8021028C
    bge @L_8021026C
    cmpwi r0, 0x1
    beq @L_8021027C
    bge @L_80210284
    b @L_802102A4
@L_8021026C:
    cmpwi r0, 0x5
    beq @L_8021029C
    bge @L_802102A4
    b @L_80210294
@L_8021027C:
    li r3, 0x0
    b @L_802102A8
@L_80210284:
    li r3, 0x1
    b @L_802102A8
@L_8021028C:
    li r3, 0x2
    b @L_802102A8
@L_80210294:
    li r3, 0x3
    b @L_802102A8
@L_8021029C:
    li r3, 0x4
    b @L_802102A8
@L_802102A4:
    li r3, 0x0
@L_802102A8:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
