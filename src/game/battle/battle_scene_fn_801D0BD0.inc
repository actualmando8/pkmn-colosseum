    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmpwi r5, 0x2
    stw r0, 0x14(r1)
    beq @L_801D0C14
    bge @L_801D0BF8
    cmpwi r5, 0x0
    beq @L_801D0C04
    bge @L_801D0C0C
    b @L_801D0C20
@L_801D0BF8:
    cmpwi r5, 0x4
    bge @L_801D0C20
    b @L_801D0C1C
@L_801D0C04:
    bl fn_801D0DB0
    b @L_801D0C20
@L_801D0C0C:
    bl fn_801D0C30
    b @L_801D0C20
@L_801D0C14:
    bl fn_801D0C30
    b @L_801D0C20
@L_801D0C1C:
    bl fn_801D0C30
@L_801D0C20:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
