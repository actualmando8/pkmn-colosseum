    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r3, -32729
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    addi r31, r3, 22760
    stw r30, 0x28(r1)
@L_801CDB20:
    lwz r6, -21580(r13)
    lwz r0, 0xc(r6)
    cmplwi r0, 0x32
    bgt @L_801CF2F8
    lis r3, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, -8536
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r4, 0x0
    li r0, 0x32
    stw r4, 0x28(r6)
    lwz r3, -21580(r13)
    stw r0, 0x60(r3)
    lwz r3, -21580(r13)
    stb r4, 0x40(r3)
    bl fn_800056D4
    lwz r4, -21580(r13)
    li r0, 0x0
    stb r3, 0x3f(r4)
    lwz r3, -21580(r13)
    stb r0, 0x3c(r3)
    lwz r3, -21580(r13)
    lbz r3, 0x3f(r3)
    bl fn_800056C4
    lwz r3, -21580(r13)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x3
    beq @L_801CDBE0
    bge @L_801CDBA8
    cmpwi r0, 0x1
    bge @L_801CDBB4
    b @L_801CDBE0
@L_801CDBA8:
    cmpwi r0, 0x5
    bge @L_801CDBE0
    b @L_801CDBC0
@L_801CDBB4:
    li r0, 0x0
    stb r0, 0x41(r3)
    b @L_801CDBE0
@L_801CDBC0:
    li r3, 0x0
    li r4, 0x4
    bl fn_80135168
    cmplwi r3, 0x0
    bne @L_801CDBE0
    lwz r3, -21580(r13)
    li r0, 0x9
    stw r0, 0x0(r3)
@L_801CDBE0:
    lwz r3, -21580(r13)
    li r4, 0x0
    li r0, 0x1
    stw r4, 0x24(r3)
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    lwz r3, 0x14(r6)
    addi r4, r6, 0x18
    addi r5, r6, 0x1c
    bl fn_800B3078
    cmpwi r3, 0x0
    mr r30, r3
    bne @L_801CDC24
    lwz r3, -21580(r13)
    lwz r3, 0x14(r3)
    bl fn_800B0358
@L_801CDC24:
    lwz r4, -21580(r13)
    cmpwi r30, 0x0
    stw r3, 0x28(r4)
    beq @L_801CDC4C
    bge @L_801CDC54
    cmpwi r30, -0x1
    bge @L_801CDC44
    b @L_801CDC54
@L_801CDC44:
    li r4, 0x1
    b @L_801CDC8C
@L_801CDC4C:
    li r4, 0x2
    b @L_801CDC8C
@L_801CDC54:
    lwz r4, -21580(r13)
    lwz r3, 0x24(r4)
    addi r0, r3, 0x1
    cmpwi r3, 0x6
    stw r0, 0x24(r4)
    bge @L_801CDC80
    lwz r3, -21580(r13)
    li r0, -0x1
    li r4, 0x1
    stw r0, 0x28(r3)
    b @L_801CDC8C
@L_801CDC80:
    lwz r3, -21580(r13)
    li r4, 0x2b
    stw r30, 0x4(r3)
@L_801CDC8C:
    lwz r3, -21580(r13)
    stw r4, 0xc(r3)
    b @L_801CF2F8
    lis r4, -32739
    lwz r3, 0x14(r6)
    addi r5, r4, 128
    lwz r4, 0x54(r6)
    li r6, 0x0
    bl fn_800B373C
    cmpwi r3, 0x0
    bne @L_801CDCC4
    lwz r3, -21580(r13)
    lwz r3, 0x14(r3)
    bl fn_800B0358
@L_801CDCC4:
    lwz r4, -21580(r13)
    li r0, 0x3
    stw r3, 0x28(r4)
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    lwz r3, 0x28(r6)
    cmpwi r3, -0x1
    beq @L_801CDCEC
    b @L_801CDCFC
@L_801CDCEC:
    lwz r3, 0x14(r6)
    bl fn_800B0358
    lwz r4, -21580(r13)
    stw r3, 0x28(r4)
@L_801CDCFC:
    cmpwi r3, -0x6
    beq @L_801CDD34
    bge @L_801CDD14
    cmpwi r3, -0xd
    beq @L_801CDD34
    b @L_801CDD3C
@L_801CDD14:
    cmpwi r3, 0x0
    beq @L_801CDD34
    bge @L_801CDD3C
    cmpwi r3, -0x1
    bge @L_801CDD2C
    b @L_801CDD3C
@L_801CDD2C:
    li r0, 0x3
    b @L_801CDD48
@L_801CDD34:
    li r0, 0x4
    b @L_801CDD48
@L_801CDD3C:
    lwz r4, -21580(r13)
    li r0, 0x2b
    stw r3, 0x4(r4)
@L_801CDD48:
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    lwz r3, 0x14(r6)
    li r4, 0x0
    bl fn_800B2F84
    cmpwi r3, 0x0
    bne @L_801CDD74
    lwz r3, -21580(r13)
    lwz r3, 0x14(r3)
    bl fn_800B0358
@L_801CDD74:
    lwz r4, -21580(r13)
    li r0, 0x5
    stw r3, 0x28(r4)
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    lwz r3, 0x28(r6)
    cmpwi r3, -0x1
    beq @L_801CDD9C
    b @L_801CDDAC
@L_801CDD9C:
    lwz r3, 0x14(r6)
    bl fn_800B0358
    lwz r4, -21580(r13)
    stw r3, 0x28(r4)
@L_801CDDAC:
    lwz r4, -21580(r13)
    lwz r0, 0x1c(r4)
    cmpwi r0, 0x2000
    beq @L_801CDDC0
    li r3, -0x6
@L_801CDDC0:
    cmpwi r3, -0x6
    beq @L_801CDE70
    bge @L_801CDDD8
    cmpwi r3, -0xd
    beq @L_801CDE70
    b @L_801CDEA8
@L_801CDDD8:
    cmpwi r3, 0x0
    beq @L_801CDDF8
    bge @L_801CDEA8
    cmpwi r3, -0x1
    bge @L_801CDDF0
    b @L_801CDEA8
@L_801CDDF0:
    li r4, 0x5
    b @L_801CDEB0
@L_801CDDF8:
    lwz r0, 0x0(r4)
    cmpwi r0, 0x9
    beq @L_801CDE68
    bge @L_801CDE14
    cmpwi r0, 0x3
    beq @L_801CDE68
    b @L_801CDE1C
@L_801CDE14:
    cmpwi r0, 0xb
    beq @L_801CDE68
@L_801CDE1C:
    lbz r0, 0x41(r4)
    cmplwi r0, 0x0
    beq @L_801CDE68
    lwz r3, 0x14(r4)
    addi r4, r1, 0x18
    bl fn_800B5AE0
    lwz r6, -21580(r13)
    lwz r4, 0x18(r1)
    lwz r0, 0x48(r6)
    lwz r5, 0x1c(r1)
    lwz r3, 0x4c(r6)
    xor r0, r4, r0
    xor r3, r5, r3
    or. r0, r3, r0
    beq @L_801CDE68
    li r0, 0x10
    li r4, 0x2b
    stw r0, 0x4(r6)
    b @L_801CDEB0
@L_801CDE68:
    li r4, 0x9
    b @L_801CDEB0
@L_801CDE70:
    lwz r0, 0x0(r4)
    cmpwi r0, 0x9
    beq @L_801CDE8C
    bge @L_801CDEA8
    cmpwi r0, 0x1
    beq @L_801CDE8C
    b @L_801CDEA8
@L_801CDE8C:
    li r3, 0x7
    li r0, 0x6
    stw r3, 0x4(r4)
    li r4, 0x30
    lwz r3, -21580(r13)
    stw r0, 0x10(r3)
    b @L_801CDEB0
@L_801CDEA8:
    stw r3, 0x4(r4)
    li r4, 0x2b
@L_801CDEB0:
    lwz r3, -21580(r13)
    stw r4, 0xc(r3)
    b @L_801CF2F8
    lwz r0, 0x4(r6)
    rlwinm. r0, r0, 0, 24, 31
    beq @L_801CDF18
    lwz r3, 0x14(r6)
    li r4, 0x0
    bl fn_800B41C0
    lwz r3, -21580(r13)
    lbz r0, 0x3c(r3)
    cmplwi r0, 0x0
    beq @L_801CDEF4
    li r0, 0x0
    li r5, 0x2b
    stw r0, 0x4(r3)
    b @L_801CDF24
@L_801CDEF4:
    li r4, 0x7
    li r0, 0x4
    stw r4, 0x60(r3)
    li r5, 0x30
    lwz r3, -21580(r13)
    stw r0, 0x4(r3)
    lwz r3, -21580(r13)
    stw r4, 0x10(r3)
    b @L_801CDF24
@L_801CDF18:
    li r0, 0x0
    li r5, 0x2b
    stw r0, 0x4(r6)
@L_801CDF24:
    lwz r3, -21580(r13)
    stw r5, 0xc(r3)
    b @L_801CF2F8
    bl fn_800056D4
    lwz r3, -21580(r13)
    li r4, 0x0
    lwz r3, 0x14(r3)
    bl fn_800B41C0
    lwz r4, -21580(r13)
    lbz r0, 0x3c(r4)
    cmplwi r0, 0x0
    beq @L_801CDF60
    stw r3, 0x4(r4)
    li r0, 0x2b
    b @L_801CDF7C
@L_801CDF60:
    cmpwi r3, 0x0
    bne @L_801CDF70
    lwz r3, 0x14(r4)
    bl fn_800B0358
@L_801CDF70:
    lwz r4, -21580(r13)
    li r0, 0x8
    stw r3, 0x28(r4)
@L_801CDF7C:
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    lwz r3, 0x28(r6)
    cmpwi r3, -0x1
    beq @L_801CDF98
    b @L_801CDFA8
@L_801CDF98:
    lwz r3, 0x14(r6)
    bl fn_800B0358
    lwz r4, -21580(r13)
    stw r3, 0x28(r4)
@L_801CDFA8:
    cmpwi r3, 0x0
    beq @L_801CDFC8
    bge @L_801CDFFC
    cmpwi r3, -0x1
    bge @L_801CDFC0
    b @L_801CDFFC
@L_801CDFC0:
    li r5, 0x8
    b @L_801CE008
@L_801CDFC8:
    lwz r3, -21580(r13)
    lbz r3, 0x3f(r3)
    bl fn_800056C4
    lwz r3, -21580(r13)
    li r4, 0x32
    li r0, 0x9
    li r5, 0x30
    stw r4, 0x60(r3)
    lwz r3, -21580(r13)
    stw r0, 0x4(r3)
    lwz r3, -21580(r13)
    stw r0, 0x10(r3)
    b @L_801CE008
@L_801CDFFC:
    lwz r4, -21580(r13)
    li r5, 0x2b
    stw r3, 0x4(r4)
@L_801CE008:
    lwz r3, -21580(r13)
    stw r5, 0xc(r3)
    b @L_801CF2F8
    bl fn_801CFD08
    lwz r4, -21580(r13)
    stw r3, 0xc(r4)
    b @L_801CF2F8
    lwz r0, 0x4(r6)
    cmpwi r0, 0x0
    bne @L_801CE040
    li r0, 0x0
    li r5, 0x2b
    stw r0, 0x4(r6)
    b @L_801CE064
@L_801CE040:
    li r0, 0x3
    li r4, 0xd
    stw r0, 0x4(r6)
    li r0, 0xb
    li r5, 0x30
    lwz r3, -21580(r13)
    stw r4, 0x60(r3)
    lwz r3, -21580(r13)
    stw r0, 0x10(r3)
@L_801CE064:
    lwz r3, -21580(r13)
    stw r5, 0xc(r3)
    b @L_801CF2F8
    bl fn_800056D4
    lwz r3, -21580(r13)
    addi r4, r31, 0x3a00
    li r5, 0x0
    lwz r3, 0x14(r3)
    bl fn_800B5228
    cmpwi r3, 0x0
    bne @L_801CE09C
    lwz r3, -21580(r13)
    lwz r3, 0x14(r3)
    bl fn_800B0358
@L_801CE09C:
    lwz r4, -21580(r13)
    li r0, 0xc
    stw r3, 0x28(r4)
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    lwz r3, 0x28(r6)
    cmpwi r3, -0x1
    beq @L_801CE0C4
    b @L_801CE0D4
@L_801CE0C4:
    lwz r3, 0x14(r6)
    bl fn_800B0358
    lwz r4, -21580(r13)
    stw r3, 0x28(r4)
@L_801CE0D4:
    cmpwi r3, -0x1
    beq @L_801CE0F8
    bge @L_801CE0EC
    cmpwi r3, -0x4
    beq @L_801CE100
    b @L_801CE128
@L_801CE0EC:
    cmpwi r3, 0x1
    bge @L_801CE128
    b @L_801CE100
@L_801CE0F8:
    li r5, 0xc
    b @L_801CE134
@L_801CE100:
    lwz r3, -21580(r13)
    lwz r0, 0x0(r3)
    cmpwi r0, 0xc
    bne @L_801CE120
    li r0, 0xa
    li r5, 0x2b
    stw r0, 0x4(r3)
    b @L_801CE134
@L_801CE120:
    li r5, 0xd
    b @L_801CE134
@L_801CE128:
    lwz r4, -21580(r13)
    li r5, 0x2b
    stw r3, 0x4(r4)
@L_801CE134:
    lwz r3, -21580(r13)
    stw r5, 0xc(r3)
    b @L_801CF2F8
    lwz r3, 0x14(r6)
    addi r4, r31, 0x3a00
    addi r6, r6, 0x8c
    lis r5, 0x6
    li r7, 0x0
    bl fn_800B4774
    cmpwi r3, 0x0
    bne @L_801CE16C
    lwz r3, -21580(r13)
    lwz r3, 0x14(r3)
    bl fn_800B0358
@L_801CE16C:
    lwz r4, -21580(r13)
    li r0, 0xe
    stw r3, 0x28(r4)
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    lwz r3, 0x28(r6)
    cmpwi r3, -0x1
    beq @L_801CE194
    b @L_801CE1A4
@L_801CE194:
    lwz r3, 0x14(r6)
    bl fn_800B0358
    lwz r4, -21580(r13)
    stw r3, 0x28(r4)
@L_801CE1A4:
    cmpwi r3, 0x0
    beq @L_801CE1C4
    bge @L_801CE1D8
    cmpwi r3, -0x1
    bge @L_801CE1BC
    b @L_801CE1D8
@L_801CE1BC:
    li r5, 0xe
    b @L_801CE1E4
@L_801CE1C4:
    lwz r3, -21580(r13)
    li r0, 0x1
    li r5, 0xf
    stb r0, 0x3e(r3)
    b @L_801CE1E4
@L_801CE1D8:
    lwz r4, -21580(r13)
    li r5, 0x2b
    stw r3, 0x4(r4)
@L_801CE1E4:
    lwz r3, -21580(r13)
    stw r5, 0xc(r3)
    b @L_801CF2F8
    lwz r3, 0x14(r6)
    addi r5, r1, 0x8
    lwz r4, 0x90(r6)
    bl fn_800B5BA4
    lbz r0, 0x8(r1)
    li r6, 0x0
    lwz r4, -21580(r13)
    ori r0, r0, 0x18
    stb r0, 0x8(r1)
    rlwinm r5, r0, 0, 24, 31
    lwz r3, 0x14(r4)
    lwz r4, 0x90(r4)
    bl fn_800B5BE4
    cmpwi r3, 0x0
    bne @L_801CE238
    lwz r3, -21580(r13)
    lwz r3, 0x14(r3)
    bl fn_800B0358
@L_801CE238:
    lwz r4, -21580(r13)
    li r0, 0x10
    stw r3, 0x28(r4)
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    lwz r3, 0x28(r6)
    cmpwi r3, -0x1
    beq @L_801CE260
    b @L_801CE270
@L_801CE260:
    lwz r3, 0x14(r6)
    bl fn_800B0358
    lwz r4, -21580(r13)
    stw r3, 0x28(r4)
@L_801CE270:
    cmpwi r3, 0x0
    beq @L_801CE290
    bge @L_801CE298
    cmpwi r3, -0x1
    bge @L_801CE288
    b @L_801CE298
@L_801CE288:
    li r0, 0x10
    b @L_801CE2A4
@L_801CE290:
    li r0, 0x11
    b @L_801CE2A4
@L_801CE298:
    lwz r4, -21580(r13)
    li r0, 0x2b
    stw r3, 0x4(r4)
@L_801CE2A4:
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    lwz r30, 0x50(r6)
    li r4, 0x0
    li r5, 0x6000
    mr r3, r30
    bl memset
    mr r3, r30
    addi r4, r31, 0x3a14
    bl fn_800CA968
    addi r3, r30, 0x20
    addi r4, r31, 0x3a28
    bl fn_800CA968
    addi r3, r30, 0x40
    addi r4, r31, 0x0
    li r5, 0x1800
    bl memcpy
    addi r3, r30, 0x1840
    addi r4, r31, 0x1800
    li r5, 0x2200
    bl memcpy
    li r0, 0xe9
    mr r3, r30
    mtctr r0
@L_801CE308:
    lwz r4, 0x3a40(r30)
    lwz r0, 0x0(r3)
    add r0, r4, r0
    stw r0, 0x3a40(r30)
    lwz r4, 0x3a40(r30)
    lwz r0, 0x4(r3)
    add r0, r4, r0
    stw r0, 0x3a40(r30)
    lwz r4, 0x3a40(r30)
    lwz r0, 0x8(r3)
    add r0, r4, r0
    stw r0, 0x3a40(r30)
    lwz r4, 0x3a40(r30)
    lwz r0, 0xc(r3)
    add r0, r4, r0
    stw r0, 0x3a40(r30)
    lwz r4, 0x3a40(r30)
    lwz r0, 0x10(r3)
    add r0, r4, r0
    stw r0, 0x3a40(r30)
    lwz r4, 0x3a40(r30)
    lwz r0, 0x14(r3)
    add r0, r4, r0
    stw r0, 0x3a40(r30)
    lwz r4, 0x3a40(r30)
    lwz r0, 0x18(r3)
    add r0, r4, r0
    stw r0, 0x3a40(r30)
    lwz r4, 0x3a40(r30)
    lwz r0, 0x1c(r3)
    add r0, r4, r0
    stw r0, 0x3a40(r30)
    lwz r4, 0x3a40(r30)
    lwz r0, 0x20(r3)
    add r0, r4, r0
    stw r0, 0x3a40(r30)
    lwz r4, 0x3a40(r30)
    lwz r0, 0x24(r3)
    add r0, r4, r0
    stw r0, 0x3a40(r30)
    lwz r4, 0x3a40(r30)
    lwz r0, 0x28(r3)
    add r0, r4, r0
    stw r0, 0x3a40(r30)
    lwz r4, 0x3a40(r30)
    lwz r0, 0x2c(r3)
    add r0, r4, r0
    stw r0, 0x3a40(r30)
    lwz r4, 0x3a40(r30)
    lwz r0, 0x30(r3)
    add r0, r4, r0
    stw r0, 0x3a40(r30)
    lwz r4, 0x3a40(r30)
    lwz r0, 0x34(r3)
    add r0, r4, r0
    stw r0, 0x3a40(r30)
    lwz r4, 0x3a40(r30)
    lwz r0, 0x38(r3)
    add r0, r4, r0
    stw r0, 0x3a40(r30)
    lwz r0, 0x3c(r3)
    addi r3, r3, 0x40
    lwz r4, 0x3a40(r30)
    add r0, r4, r0
    stw r0, 0x3a40(r30)
    bdnz @L_801CE308
    lwz r3, -21580(r13)
    li r0, 0x12
    stw r0, 0xc(r3)
    b @L_801CF2F8
    lbz r0, 0x40(r6)
    addi r3, r6, 0x8c
    lwz r4, 0x50(r6)
    li r5, 0x6000
    cmplwi r0, 0x0
    beq @L_801CE43C
    li r5, 0x4000
@L_801CE43C:
    li r6, 0x0
    li r7, 0x0
    bl fn_800B5070
    cmpwi r3, 0x0
    bne @L_801CE45C
    lwz r3, -21580(r13)
    lwz r3, 0x14(r3)
    bl fn_800B0358
@L_801CE45C:
    lwz r4, -21580(r13)
    li r0, 0x13
    stw r3, 0x28(r4)
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    lwz r3, 0x28(r6)
    cmpwi r3, -0x1
    beq @L_801CE484
    b @L_801CE494
@L_801CE484:
    lwz r3, 0x14(r6)
    bl fn_800B0358
    lwz r4, -21580(r13)
    stw r3, 0x28(r4)
@L_801CE494:
    cmpwi r3, 0x0
    beq @L_801CE4B4
    bge @L_801CE4D4
    cmpwi r3, -0x1
    bge @L_801CE4AC
    b @L_801CE4D4
@L_801CE4AC:
    li r0, 0x13
    b @L_801CE4E0
@L_801CE4B4:
    lwz r3, -21580(r13)
    lbz r0, 0x40(r3)
    cmplwi r0, 0x0
    beq @L_801CE4CC
    li r0, 0x2c
    b @L_801CE4E0
@L_801CE4CC:
    li r0, 0x14
    b @L_801CE4E0
@L_801CE4D4:
    lwz r4, -21580(r13)
    li r0, 0x2b
    stw r3, 0x4(r4)
@L_801CE4E0:
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    lwz r30, 0x50(r6)
    li r4, 0x0
    li r5, 0x2000
    mr r3, r30
    bl memset
    li r3, 0x1
    li r6, 0x0
    stb r3, 0x0(r30)
    li r0, 0x15
    stb r3, 0x1(r30)
    stb r6, 0x2(r30)
    stb r6, 0x3(r30)
    lwz r5, 0x0(r30)
    lwz r3, 0x4(r30)
    lwz r4, 0x8(r30)
    add r5, r5, r3
    lwz r3, 0xc(r30)
    add r5, r5, r4
    lwz r4, 0x10(r30)
    add r5, r5, r3
    lwz r3, 0x14(r30)
    add r5, r5, r4
    lwz r4, 0x18(r30)
    add r5, r5, r3
    lwz r3, 0x1c(r30)
    add r5, r5, r4
    add r5, r5, r3
    neg r3, r5
    stw r3, 0xc(r30)
    lwz r3, -21580(r13)
    stw r6, 0x20(r3)
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    lis r3, 0x2
    lwz r5, 0x20(r6)
    subi r0, r3, 0x2000
    lwz r4, 0x50(r6)
    mullw r8, r5, r0
    addi r3, r6, 0x8c
    li r5, 0x2000
    li r7, 0x0
    addi r6, r8, 0x6000
    bl fn_800B5070
    cmpwi r3, 0x0
    bne @L_801CE5B0
    lwz r3, -21580(r13)
    lwz r3, 0x14(r3)
    bl fn_800B0358
@L_801CE5B0:
    lwz r4, -21580(r13)
    li r0, 0x16
    stw r3, 0x28(r4)
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    lwz r3, 0x28(r6)
    cmpwi r3, -0x1
    beq @L_801CE5D8
    b @L_801CE5E8
@L_801CE5D8:
    lwz r3, 0x14(r6)
    bl fn_800B0358
    lwz r4, -21580(r13)
    stw r3, 0x28(r4)
@L_801CE5E8:
    cmpwi r3, 0x0
    beq @L_801CE608
    bge @L_801CE630
    cmpwi r3, -0x1
    bge @L_801CE600
    b @L_801CE630
@L_801CE600:
    li r0, 0x16
    b @L_801CE63C
@L_801CE608:
    lwz r4, -21580(r13)
    lwz r3, 0x20(r4)
    addi r0, r3, 0x1
    cmplwi r0, 0x3
    stw r0, 0x20(r4)
    bge @L_801CE628
    li r0, 0x15
    b @L_801CE63C
@L_801CE628:
    li r0, 0x17
    b @L_801CE63C
@L_801CE630:
    lwz r4, -21580(r13)
    li r0, 0x2b
    stw r3, 0x4(r4)
@L_801CE63C:
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    lwz r3, 0x14(r6)
    addi r5, r6, 0xa0
    lwz r4, 0x90(r6)
    bl fn_800B5530
    cmpwi r3, 0x0
    bne @L_801CE818
    lwz r3, -21580(r13)
    li r4, 0x0
    li r0, 0x40
    stw r4, 0xd8(r3)
    lwz r3, -21580(r13)
    stw r0, 0xd0(r3)
    lwz r3, -21580(r13)
    lbz r0, 0xce(r3)
    rlwinm r0, r0, 0, 0, 29
    ori r0, r0, 0x2
    stb r0, 0xce(r3)
    lwz r3, -21580(r13)
    lbz r0, 0xce(r3)
    rlwinm r0, r0, 0, 30, 28
    stb r0, 0xce(r3)
    lwz r3, -21580(r13)
    lhz r0, 0xd4(r3)
    rlwinm r0, r0, 0, 0, 29
    ori r0, r0, 0x1
    sth r0, 0xd4(r3)
    lwz r3, -21580(r13)
    lhz r0, 0xd6(r3)
    rlwinm r0, r0, 0, 0, 29
    ori r0, r0, 0x2
    sth r0, 0xd6(r3)
    lwz r3, -21580(r13)
    lhz r0, 0xd4(r3)
    rlwinm r0, r0, 0, 30, 27
    ori r0, r0, 0x4
    sth r0, 0xd4(r3)
    lwz r3, -21580(r13)
    lhz r0, 0xd6(r3)
    rlwinm r0, r0, 0, 30, 27
    ori r0, r0, 0x8
    sth r0, 0xd6(r3)
    lwz r3, -21580(r13)
    lhz r0, 0xd4(r3)
    rlwinm r0, r0, 0, 28, 25
    ori r0, r0, 0x10
    sth r0, 0xd4(r3)
    lwz r3, -21580(r13)
    lhz r0, 0xd6(r3)
    rlwinm r0, r0, 0, 28, 25
    ori r0, r0, 0x20
    sth r0, 0xd6(r3)
    lwz r3, -21580(r13)
    lhz r0, 0xd4(r3)
    rlwinm r0, r0, 0, 26, 23
    ori r0, r0, 0x40
    sth r0, 0xd4(r3)
    lwz r3, -21580(r13)
    lhz r0, 0xd6(r3)
    rlwinm r0, r0, 0, 26, 23
    ori r0, r0, 0x80
    sth r0, 0xd6(r3)
    lwz r3, -21580(r13)
    lhz r0, 0xd4(r3)
    rlwinm r0, r0, 0, 24, 21
    ori r0, r0, 0x100
    sth r0, 0xd4(r3)
    lwz r3, -21580(r13)
    lhz r0, 0xd6(r3)
    rlwinm r0, r0, 0, 24, 21
    ori r0, r0, 0x200
    sth r0, 0xd6(r3)
    lwz r3, -21580(r13)
    lhz r0, 0xd4(r3)
    rlwinm r0, r0, 0, 22, 19
    ori r0, r0, 0x400
    sth r0, 0xd4(r3)
    lwz r3, -21580(r13)
    lhz r0, 0xd6(r3)
    rlwinm r0, r0, 0, 22, 19
    ori r0, r0, 0x800
    sth r0, 0xd6(r3)
    lwz r3, -21580(r13)
    li r6, 0x0
    lhz r0, 0xd4(r3)
    rlwinm r0, r0, 0, 20, 17
    ori r0, r0, 0x1000
    sth r0, 0xd4(r3)
    lwz r3, -21580(r13)
    lhz r0, 0xd6(r3)
    rlwinm r0, r0, 0, 20, 17
    ori r0, r0, 0x2000
    sth r0, 0xd6(r3)
    lwz r3, -21580(r13)
    lhz r0, 0xd4(r3)
    rlwinm r0, r0, 0, 18, 15
    ori r0, r0, 0x4000
    sth r0, 0xd4(r3)
    lwz r3, -21580(r13)
    lhz r0, 0xd6(r3)
    rlwinm r0, r0, 0, 18, 15
    ori r0, r0, 0x8000
    sth r0, 0xd6(r3)
    lwz r4, -21580(r13)
    lwz r3, 0x14(r4)
    addi r5, r4, 0xa0
    lwz r4, 0x90(r4)
    bl fn_800B565C
    cmpwi r3, 0x0
    bne @L_801CE808
    lwz r3, -21580(r13)
    lwz r3, 0x14(r3)
    bl fn_800B0358
@L_801CE808:
    lwz r4, -21580(r13)
    li r0, 0x18
    stw r3, 0x28(r4)
    b @L_801CE824
@L_801CE818:
    lwz r4, -21580(r13)
    li r0, 0x2b
    stw r3, 0x4(r4)
@L_801CE824:
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    lwz r3, 0x28(r6)
    cmpwi r3, -0x1
    beq @L_801CE840
    b @L_801CE850
@L_801CE840:
    lwz r3, 0x14(r6)
    bl fn_800B0358
    lwz r4, -21580(r13)
    stw r3, 0x28(r4)
@L_801CE850:
    cmpwi r3, 0x0
    beq @L_801CE870
    bge @L_801CE8F0
    cmpwi r3, -0x1
    bge @L_801CE868
    b @L_801CE8F0
@L_801CE868:
    li r5, 0x18
    b @L_801CE8FC
@L_801CE870:
    lwz r4, -21580(r13)
    lwz r0, 0x0(r4)
    cmpwi r0, 0x9
    bne @L_801CE8D4
    lwz r30, 0x50(r4)
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    li r0, 0x3bfa
    addi r5, r30, 0x4
    subi r4, r3, 0x4
    mtctr r0
@L_801CE8A0:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_801CE8A0
    lwz r3, -21580(r13)
    li r4, 0x0
    li r0, -0x1
    li r5, 0x25
    stw r4, 0x2c(r3)
    lwz r3, -21580(r13)
    stw r0, 0x20(r3)
    b @L_801CE8FC
@L_801CE8D4:
    li r3, 0xb
    li r0, 0x2
    stw r3, 0x4(r4)
    li r5, 0x2c
    lwz r3, -21580(r13)
    stw r0, 0x8(r3)
    b @L_801CE8FC
@L_801CE8F0:
    lwz r4, -21580(r13)
    li r5, 0x2b
    stw r3, 0x4(r4)
@L_801CE8FC:
    lwz r3, -21580(r13)
    stw r5, 0xc(r3)
    b @L_801CF2F8
    li r0, 0x1a
    stw r0, 0xc(r6)
    b @L_801CF2F8
    lwz r4, 0x50(r6)
    addi r3, r6, 0x8c
    li r5, 0x4000
    li r6, 0x0
    li r7, 0x0
    bl fn_800B4C7C
    cmpwi r3, 0x0
    bne @L_801CE940
    lwz r3, -21580(r13)
    lwz r3, 0x14(r3)
    bl fn_800B0358
@L_801CE940:
    lwz r4, -21580(r13)
    li r0, 0x1b
    stw r3, 0x28(r4)
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    lwz r3, 0x28(r6)
    li r30, 0x0
    cmpwi r3, -0x1
    beq @L_801CE96C
    b @L_801CE97C
@L_801CE96C:
    lwz r3, 0x14(r6)
    bl fn_800B0358
    lwz r4, -21580(r13)
    stw r3, 0x28(r4)
@L_801CE97C:
    cmpwi r3, 0x0
    beq @L_801CE99C
    bge @L_801CEA54
    cmpwi r3, -0x1
    bge @L_801CE994
    b @L_801CEA54
@L_801CE994:
    li r0, 0x1b
    b @L_801CEA60
@L_801CE99C:
    lwz r6, -21580(r13)
    li r0, 0xe9
    lwz r4, 0x50(r6)
    mr r5, r4
    mtctr r0
@L_801CE9B0:
    lwz r3, 0x0(r5)
    lwz r0, 0x4(r5)
    add r30, r30, r3
    lwz r3, 0x8(r5)
    add r30, r30, r0
    lwz r0, 0xc(r5)
    add r30, r30, r3
    lwz r3, 0x10(r5)
    add r30, r30, r0
    lwz r0, 0x14(r5)
    add r30, r30, r3
    lwz r3, 0x18(r5)
    add r30, r30, r0
    lwz r0, 0x1c(r5)
    add r30, r30, r3
    lwz r3, 0x20(r5)
    add r30, r30, r0
    lwz r0, 0x24(r5)
    add r30, r30, r3
    lwz r3, 0x28(r5)
    add r30, r30, r0
    lwz r0, 0x2c(r5)
    add r30, r30, r3
    lwz r3, 0x30(r5)
    add r30, r30, r0
    lwz r0, 0x34(r5)
    add r30, r30, r3
    lwz r3, 0x38(r5)
    add r30, r30, r0
    lwz r0, 0x3c(r5)
    add r30, r30, r3
    addi r5, r5, 0x40
    add r30, r30, r0
    bdnz @L_801CE9B0
    lwz r0, 0x3a40(r4)
    cmplw r30, r0
    beq @L_801CEA4C
    li r0, 0x1
    stb r0, 0x40(r6)
@L_801CEA4C:
    li r0, 0x1c
    b @L_801CEA60
@L_801CEA54:
    lwz r4, -21580(r13)
    li r0, 0x2b
    stw r3, 0x4(r4)
@L_801CEA60:
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    li r0, 0x0
    stw r0, 0x30(r6)
    lwz r3, -21580(r13)
    stw r0, 0x20(r3)
    lwz r3, -21580(r13)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x3
    beq @L_801CEAC0
    bge @L_801CEA9C
    cmpwi r0, 0x1
    bge @L_801CEAA8
    b @L_801CEAC8
@L_801CEA9C:
    cmpwi r0, 0x8
    beq @L_801CEAC0
    b @L_801CEAC8
@L_801CEAA8:
    lbz r0, 0x40(r3)
    cmplwi r0, 0x0
    beq @L_801CEAC0
    li r0, 0x5
    stw r0, 0x4(r3)
    b @L_801CEAD0
@L_801CEAC0:
    li r4, 0x1d
    b @L_801CEAE0
@L_801CEAC8:
    li r0, 0x2
    stw r0, 0x4(r3)
@L_801CEAD0:
    lwz r3, -21580(r13)
    li r0, 0x1d
    li r4, 0x30
    stw r0, 0x10(r3)
@L_801CEAE0:
    lwz r3, -21580(r13)
    stw r4, 0xc(r3)
    b @L_801CF2F8
    lwz r3, 0x28(r6)
    cmpwi r3, -0x1
    beq @L_801CEAFC
    b @L_801CEB0C
@L_801CEAFC:
    lwz r3, 0x14(r6)
    bl fn_800B0358
    lwz r4, -21580(r13)
    stw r3, 0x28(r4)
@L_801CEB0C:
    cmpwi r3, 0x0
    beq @L_801CEB2C
    bge @L_801CEB54
    cmpwi r3, -0x1
    bge @L_801CEB24
    b @L_801CEB54
@L_801CEB24:
    li r0, 0x22
    b @L_801CEB60
@L_801CEB2C:
    lwz r4, -21580(r13)
    lwz r3, 0x20(r4)
    addi r0, r3, 0x1
    cmplwi r0, 0x3
    stw r0, 0x20(r4)
    bge @L_801CEB4C
    li r0, 0x1d
    b @L_801CEB60
@L_801CEB4C:
    li r0, 0x1f
    b @L_801CEB60
@L_801CEB54:
    lwz r4, -21580(r13)
    li r0, 0x2b
    stw r3, 0x4(r4)
@L_801CEB60:
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    lis r3, 0x2
    lwz r4, 0x20(r6)
    subi r0, r3, 0x2000
    lwz r7, 0x50(r6)
    mullw r8, r4, r0
    rlwinm r4, r4, 9, 0, 22
    addi r3, r6, 0x8c
    addis r0, r4, 0x2
    li r5, 0x200
    add r4, r0, r7
    addi r6, r8, 0x6000
    li r7, 0x0
    subi r4, r4, 0x2000
    bl fn_800B4C7C
    cmpwi r3, 0x0
    bne @L_801CEBB8
    lwz r3, -21580(r13)
    lwz r3, 0x14(r3)
    bl fn_800B0358
@L_801CEBB8:
    lwz r4, -21580(r13)
    li r0, 0x1e
    stw r3, 0x28(r4)
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    bl fn_801CF9C8
    lwz r4, -21580(r13)
    stw r3, 0xc(r4)
    b @L_801CF2F8
    li r0, 0x21
    stw r0, 0xc(r6)
    b @L_801CF2F8
    lis r3, 0x2
    lwz r0, 0x20(r6)
    subi r5, r3, 0x2000
    lwz r4, 0x50(r6)
    mullw r8, r0, r5
    addi r3, r6, 0x8c
    li r7, 0x0
    addi r6, r8, 0x6000
    bl fn_800B4C7C
    cmpwi r3, 0x0
    bne @L_801CEC24
    lwz r3, -21580(r13)
    lwz r3, 0x14(r3)
    bl fn_800B0358
@L_801CEC24:
    lwz r4, -21580(r13)
    li r0, 0x22
    stw r3, 0x28(r4)
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    lwz r3, 0x28(r6)
    cmpwi r3, -0x1
    beq @L_801CEC4C
    b @L_801CEC5C
@L_801CEC4C:
    lwz r3, 0x14(r6)
    bl fn_800B0358
    lwz r4, -21580(r13)
    stw r3, 0x28(r4)
@L_801CEC5C:
    cmpwi r3, 0x0
    beq @L_801CEC7C
    bge @L_801CEC84
    cmpwi r3, -0x1
    bge @L_801CEC74
    b @L_801CEC84
@L_801CEC74:
    li r0, 0x22
    b @L_801CEC90
@L_801CEC7C:
    li r0, 0x23
    b @L_801CEC90
@L_801CEC84:
    lwz r4, -21580(r13)
    li r0, 0x2b
    stw r3, 0x4(r4)
@L_801CEC90:
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    bl fn_801CF7E4
    lwz r4, -21580(r13)
    stw r3, 0xc(r4)
    b @L_801CF2F8
    bl fn_801CF568
    lwz r4, -21580(r13)
    stw r3, 0xc(r4)
    b @L_801CF2F8
    bl fn_801CF320
    lwz r4, -21580(r13)
    stw r3, 0xc(r4)
    b @L_801CF2F8
    lwz r0, 0x0(r6)
    lis r3, 0x2
    subi r30, r3, 0x2000
    cmpwi r0, 0x8
    bne @L_801CECE4
    subi r30, r3, 0x4000
@L_801CECE4:
    bl fn_800056D4
    lwz r4, -21580(r13)
    lis r3, 0x2
    subi r0, r3, 0x2000
    mr r5, r30
    lwz r6, 0x20(r4)
    addi r3, r4, 0x8c
    lwz r4, 0x50(r4)
    li r7, 0x0
    mullw r6, r6, r0
    addi r6, r6, 0x6000
    bl fn_800B5070
    cmpwi r3, 0x0
    bne @L_801CED28
    lwz r3, -21580(r13)
    lwz r3, 0x14(r3)
    bl fn_800B0358
@L_801CED28:
    lwz r5, -21580(r13)
    li r4, 0x0
    li r0, 0x27
    stw r3, 0x28(r5)
    lwz r3, -21580(r13)
    stw r4, 0x38(r3)
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    lwz r3, 0x28(r6)
    cmpwi r3, -0x1
    beq @L_801CED5C
    b @L_801CED6C
@L_801CED5C:
    lwz r3, 0x14(r6)
    bl fn_800B0358
    lwz r4, -21580(r13)
    stw r3, 0x28(r4)
@L_801CED6C:
    lwz r5, -21580(r13)
    cmpwi r3, 0x0
    lwz r4, 0x38(r5)
    addi r0, r4, 0x1
    stw r0, 0x38(r5)
    beq @L_801CED9C
    bge @L_801CEE64
    cmpwi r3, -0x1
    bge @L_801CED94
    b @L_801CEE64
@L_801CED94:
    li r5, 0x27
    b @L_801CEE70
@L_801CED9C:
    lwz r4, -21580(r13)
    lwz r0, 0x0(r4)
    cmpwi r0, 0x8
    beq @L_801CEDD4
    bge @L_801CEDC8
    cmpwi r0, 0x4
    beq @L_801CEDFC
    bge @L_801CEE2C
    cmpwi r0, 0x1
    bge @L_801CEDE4
    b @L_801CEE2C
@L_801CEDC8:
    cmpwi r0, 0xc
    bge @L_801CEE2C
    b @L_801CEDFC
@L_801CEDD4:
    li r0, 0x28
    li r5, 0x31
    stw r0, 0x10(r4)
    b @L_801CEE70
@L_801CEDE4:
    li r3, 0x12
    li r0, 0x3
    stw r3, 0x4(r4)
    lwz r3, -21580(r13)
    stw r0, 0x8(r3)
    b @L_801CEE44
@L_801CEDFC:
    li r3, 0x0
    li r4, 0x1
    bl fn_80129280
    li r4, 0x4
    bl fn_80135168
    mr r30, r3
    li r3, 0x0
    li r4, 0x1
    bl fn_80129280
    addi r5, r30, 0x1
    li r4, 0x4
    bl fn_80135030
@L_801CEE2C:
    lwz r3, -21580(r13)
    li r4, 0xa
    li r0, 0x4
    stw r4, 0x4(r3)
    lwz r3, -21580(r13)
    stw r0, 0x8(r3)
@L_801CEE44:
    lwz r3, -21580(r13)
    lbz r0, 0x40(r3)
    cmplwi r0, 0x0
    beq @L_801CEE5C
    li r5, 0x11
    b @L_801CEE70
@L_801CEE5C:
    li r5, 0x2c
    b @L_801CEE70
@L_801CEE64:
    lwz r4, -21580(r13)
    li r5, 0x2b
    stw r3, 0x4(r4)
@L_801CEE70:
    lwz r3, -21580(r13)
    stw r5, 0xc(r3)
    b @L_801CF2F8
    lwz r3, 0x34(r6)
    subi r0, r3, 0x1
    cmpwi r3, 0x0
    stw r0, 0x34(r6)
    beq @L_801CEEA4
    lwz r3, -21580(r13)
    li r0, -0x1
    li r4, 0x28
    stw r0, 0x28(r3)
    b @L_801CEEB4
@L_801CEEA4:
    lwz r3, -21580(r13)
    li r0, 0x0
    li r4, 0x29
    stw r0, 0x28(r3)
@L_801CEEB4:
    lwz r3, -21580(r13)
    stw r4, 0xc(r3)
    b @L_801CF2F8
    li r3, 0x2
    bl fn_8008ABA0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801CEED8
    li r6, 0x2b
    b @L_801CEF68
@L_801CEED8:
    lwz r4, -21580(r13)
    lis r3, 0x2
    subi r0, r3, 0x2000
    li r5, 0x2000
    lwz r6, 0x20(r4)
    addi r3, r4, 0x8c
    lwz r4, 0x50(r4)
    li r7, 0x0
    mullw r6, r6, r0
    addis r4, r4, 0x2
    subi r4, r4, 0x4000
    addi r6, r6, 0x6000
    addis r6, r6, 0x2
    subi r6, r6, 0x4000
    bl fn_800B5070
    cmpwi r3, 0x0
    bne @L_801CEF28
    lwz r3, -21580(r13)
    lwz r3, 0x14(r3)
    bl fn_800B0358
@L_801CEF28:
    lwz r6, -21580(r13)
    lis r4, 0x2492
    addi r5, r4, 0x4925
    li r0, 0x0
    stw r3, 0x28(r6)
    li r6, 0x2a
    lwz r4, -21580(r13)
    lwz r3, 0x38(r4)
    mulhwu r5, r5, r3
    subf r3, r5, r3
    rlwinm r3, r3, 31, 1, 31
    add r3, r3, r5
    rlwinm r3, r3, 29, 3, 31
    stw r3, 0x38(r4)
    lwz r3, -21580(r13)
    stb r0, 0x3d(r3)
@L_801CEF68:
    lwz r3, -21580(r13)
    stw r6, 0xc(r3)
    b @L_801CF2F8
    lwz r30, 0x28(r6)
    cmpwi r30, -0x1
    beq @L_801CEF84
    b @L_801CEF98
@L_801CEF84:
    lwz r3, 0x14(r6)
    bl fn_800B0358
    lwz r4, -21580(r13)
    mr r30, r3
    stw r30, 0x28(r4)
@L_801CEF98:
    cmpwi r30, 0x0
    beq @L_801CF03C
    bge @L_801CF0BC
    cmpwi r30, -0x1
    bge @L_801CEFB0
    b @L_801CF0BC
@L_801CEFB0:
    lwz r4, -21580(r13)
    lwz r3, 0x38(r4)
    subi r0, r3, 0x1
    stw r0, 0x38(r4)
    lwz r3, -21580(r13)
    lbz r0, 0x3d(r3)
    cmplwi r0, 0x0
    bne @L_801CF034
    li r3, 0x2
    bl fn_8008ABA0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801CEFF4
    lwz r3, -21580(r13)
    addi r3, r3, 0x8c
    bl fn_800B4DC4
    li r0, 0x2b
    b @L_801CF0D0
@L_801CEFF4:
    lwz r3, -21580(r13)
    lwz r0, 0x38(r3)
    cmpwi r0, 0x3
    bge @L_801CF034
    li r3, 0x2
    bl fn_80089D74
    cmpwi r3, 0x0
    beq @L_801CF028
    lwz r3, -21580(r13)
    addi r3, r3, 0x8c
    bl fn_800B4DC4
    li r0, 0x2b
    b @L_801CF0D0
@L_801CF028:
    lwz r3, -21580(r13)
    li r0, 0x1
    stb r0, 0x3d(r3)
@L_801CF034:
    li r0, 0x2a
    b @L_801CF0D0
@L_801CF03C:
    lwz r3, -21580(r13)
    lbz r0, 0x3d(r3)
    cmplwi r0, 0x0
    bne @L_801CF054
    li r3, 0x2
    bl fn_80089D74
@L_801CF054:
    li r3, 0x0
    li r4, 0x1
    bl fn_80129280
    li r4, 0x4
    bl fn_80135168
    mr r30, r3
    li r3, 0x0
    li r4, 0x1
    bl fn_80129280
    addi r5, r30, 0x1
    li r4, 0x4
    bl fn_80135030
    lwz r3, -21580(r13)
    li r4, 0x0
    li r0, 0x4
    stw r4, 0x4(r3)
    lwz r3, -21580(r13)
    stw r0, 0x8(r3)
    lwz r3, -21580(r13)
    lbz r0, 0x40(r3)
    cmplwi r0, 0x0
    beq @L_801CF0B4
    li r0, 0x11
    b @L_801CF0D0
@L_801CF0B4:
    li r0, 0x2c
    b @L_801CF0D0
@L_801CF0BC:
    li r3, 0x1
    bl fn_80072A00
    lwz r3, -21580(r13)
    li r0, 0x2b
    stw r30, 0x4(r3)
@L_801CF0D0:
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    li r3, -0x1
    li r0, 0x2c
    stw r3, 0x8(r6)
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    lbz r0, 0x3e(r6)
    cmplwi r0, 0x0
    beq @L_801CF144
    bl fn_800057A8
    cmpwi r3, 0x4
    bne @L_801CF11C
    lwz r4, -21580(r13)
    lwz r3, 0x14(r4)
    lwz r4, 0x68(r4)
    bl fn_800B01C4
@L_801CF11C:
    lwz r3, -21580(r13)
    addi r4, r1, 0x10
    lwz r3, 0x14(r3)
    bl fn_800B5AE0
    lwz r3, -21580(r13)
    addi r3, r3, 0x8c
    bl fn_800B45E8
    lwz r3, -21580(r13)
    li r0, 0x0
    stb r0, 0x3e(r3)
@L_801CF144:
    lwz r3, -21580(r13)
    lwz r3, 0x14(r3)
    bl fn_800B3978
    lwz r3, -21580(r13)
    lbz r3, 0x3f(r3)
    bl fn_800056C4
    lwz r5, -21580(r13)
    lbz r0, 0x3c(r5)
    cmplwi r0, 0x0
    beq @L_801CF174
    li r4, 0x0
    b @L_801CF1E8
@L_801CF174:
    lwz r0, 0x0(r5)
    cmpwi r0, 0x8
    beq @L_801CF190
    bge @L_801CF198
    cmpwi r0, 0x3
    beq @L_801CF190
    b @L_801CF198
@L_801CF190:
    li r4, 0x32
    b @L_801CF1E8
@L_801CF198:
    lwz r0, 0x8(r5)
    cmpwi r0, 0x5
    bge @L_801CF1CC
    cmpwi r0, 0x1
    bge @L_801CF1B0
    b @L_801CF1CC
@L_801CF1B0:
    lwz r3, 0x10(r1)
    li r0, 0x1
    lwz r4, 0x14(r1)
    stw r4, 0x4c(r5)
    stw r3, 0x48(r5)
    lwz r3, -21580(r13)
    stb r0, 0x41(r3)
@L_801CF1CC:
    lwz r3, -21580(r13)
    li r0, 0x2d
    li r4, 0x30
    lwz r5, 0x4(r3)
    stw r5, 0x4(r3)
    lwz r3, -21580(r13)
    stw r0, 0x10(r3)
@L_801CF1E8:
    lwz r3, -21580(r13)
    stw r4, 0xc(r3)
    b @L_801CF2F8
    lwz r0, 0x0(r6)
    cmplwi r0, 0xd
    bgt @L_801CF29C
    lis r3, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, -8592
    lwzx r0, r3, r0
    mtctr r0
    bctr
    lwz r0, 0x8(r6)
    cmpwi r0, 0x4
    beq @L_801CF29C
    li r3, 0xf
    li r0, 0x32
    stw r3, 0x4(r6)
    li r4, 0x30
    lwz r3, -21580(r13)
    stw r0, 0x10(r3)
    b @L_801CF2A0
    lwz r0, 0x8(r6)
    cmpwi r0, -0x1
    bne @L_801CF29C
    lwz r0, 0x60(r6)
    cmpwi r0, 0xd
    beq @L_801CF274
    bge @L_801CF280
    cmpwi r0, 0x7
    beq @L_801CF268
    b @L_801CF280
@L_801CF268:
    li r0, 0xd
    stw r0, 0x4(r6)
    b @L_801CF288
@L_801CF274:
    li r0, 0xe
    stw r0, 0x4(r6)
    b @L_801CF288
@L_801CF280:
    li r4, 0x2e
    b @L_801CF2A0
@L_801CF288:
    lwz r3, -21580(r13)
    li r0, 0x2e
    li r4, 0x30
    stw r0, 0x10(r3)
    b @L_801CF2A0
@L_801CF29C:
    li r4, 0x32
@L_801CF2A0:
    lwz r3, -21580(r13)
    stw r4, 0xc(r3)
    b @L_801CF2F8
    li r0, 0x8
    li r4, 0x2f
    stw r0, 0x4(r6)
    li r0, 0x30
    lwz r3, -21580(r13)
    stw r4, 0x10(r3)
    lwz r3, -21580(r13)
    stw r0, 0xc(r3)
    b @L_801CF2F8
    lwz r0, 0x4(r6)
    rlwinm. r0, r0, 0, 24, 31
    beq @L_801CF2E4
    li r0, 0x5
    stw r0, 0x8(r6)
@L_801CF2E4:
    lwz r3, -21580(r13)
    li r0, 0x32
    stw r0, 0xc(r3)
    b @L_801CF2F8
    b @L_801CF308
@L_801CF2F8:
    lwz r3, -21580(r13)
    lwz r0, 0x28(r3)
    cmpwi r0, -0x1
    bne @L_801CDB20
@L_801CF308:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
