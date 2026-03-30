    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    bl fn_801653C4
    mr. r30, r3
    beq @L_8025DD60
    bl fn_801656D8
    mr r0, r3
    li r3, 0x1
    mr r29, r0
    li r4, 0x32
    li r5, 0xff
    bl fn_80165A20
    b @L_8025DD64
@L_8025DD60:
    li r29, 0x0
@L_8025DD64:
    bl fn_801653BC
    mr. r31, r3
    beq @L_8025DD78
    bl fn_801656D8
    b @L_8025DD7C
@L_8025DD78:
    li r3, 0x0
@L_8025DD7C:
    stw r30, 0x0(r28)
    stw r31, 0x4(r28)
    stw r29, 0x8(r28)
    stw r3, 0xc(r28)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
