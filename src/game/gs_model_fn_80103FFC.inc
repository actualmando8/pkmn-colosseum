    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    cmplwi r30, 0x0
    bne @L_80104028
    li r3, 0x0
    b @L_80104088
    @L_80104028:
    lhz r3, 0xac(r30)
    cmplwi r3, 0x0
    beq @L_80104048
    bl fn_800E24B0
    lhz r3, 0xac(r30)
    bl fn_800E209C
    li r0, 0x0
    stw r0, 0xb0(r30)
    @L_80104048:
    cmpwi r31, 0x0
    bgt @L_80104058
    li r3, 0x0
    b @L_80104088
    @L_80104058:
    mr r3, r31
    bl fn_800E3534
    sth r3, 0xac(r30)
    lhz r3, 0xac(r30)
    cmplwi r3, 0x0
    beq @L_8010407C
    bl fn_800E27B0
    stw r3, 0xb0(r30)
    b @L_80104084
    @L_8010407C:
    li r3, 0x0
    b @L_80104088
    @L_80104084:
    lwz r3, 0xb0(r30)
    @L_80104088:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
